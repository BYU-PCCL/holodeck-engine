// Fill out your copyright notice in the Description page of Project Settings.
#include "Holodeck.h"
#include "UAV.h"
#include <math.h>

AUAV::AUAV()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.HolodeckUAVController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// TODO: the unreal unit conversion should be derived via global settings //This is way more buggy than anticipated.
	// TODO: accept desired roll/pitch/yaw/altitude commands via HolodeckMessage
	// TODO: add changes seen in https://answers.unrealengine.com/questions/7459/question-is-120-the-engine-max-frame-rate.html
	// TODO: set deltaTick to 1/40th of a second
	// TODO: make all UAB "constants" blueprint properties
	// TODO: android needs a substepping function
	// TODO: IMU needs a substepping function
	// TODO: CAmeraSensorArray needs to publish player camera snapshots also
	// TODO: android needs to publish substepped phyx data instead of unreal nonsense
	// TODO: imu needs to publish substepped phyx data instead of unreal nonsense

	SetActorEnableCollision(true);
	OnCalculateCustomPhysics.BindUObject(this, &AUAV::SubstepTick);
}

// Called when the game starts or when spawned
void AUAV::BeginPlay()
{
	Super::BeginPlay();

	RootMesh = Cast<UStaticMeshComponent>(RootComponent);
	UAVRigidBody = RootMesh->GetBodyInstance()->GetPxRigidBody();

	Parent = Cast<UPrimitiveComponent>(RootMesh);

	World = Parent->GetWorld();
	WorldSettings = World->GetWorldSettings(false, false);
	WorldToMetersRatio = WorldSettings->WorldToMeters;
		
	// Set up the PID Controllers TODO - WHAT TO USE FOR TAU?
	RollController.setGains(UAV_ROLL_P, UAV_ROLL_I, UAV_ROLL_D, UAV_TAU_DOWN_ROLL_L);
	PitchController.setGains(UAV_PITCH_P, UAV_PITCH_I, UAV_PITCH_D, UAV_TAU_DOWN_PITCH_M);
	YawController.setGains(UAV_YAW_P, UAV_YAW_I, UAV_YAW_D, UAV_TAU_DOWN_YAW_RATE_N);
	AltitudeController.setGains(UAV_ALT_P, UAV_ALT_I, UAV_ALT_D, UAV_TAU_DOWN_FORCE_F);
}

// Called every frame
void AUAV::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	if(RootMesh){
		RootMesh->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	}
}
	
void AUAV::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
{
	UpdateForcesAndMoments(DeltaTime);
	ApplyForces();
}

FVector AUAV::RotatorToEulerInZYX(FRotator Rotator) {
	FQuat Quaternion = Rotator.Quaternion();

	float Roll = atan2(2.0 * (Quaternion.W * Quaternion.X + Quaternion.Y * Quaternion.Z), 1.0 - 2.0 * (Quaternion.X * Quaternion.X + Quaternion.Y * Quaternion.Y));
	float Pitch = asin(2.0 * (Quaternion.W * Quaternion.Y - Quaternion.Z * Quaternion.X));
	float Yaw = atan2(2.0 * (Quaternion.W * Quaternion.Z + Quaternion.X * Quaternion.Y), 1.0 - 2.0 * (Quaternion.Y * Quaternion.Z + Quaternion.Z * Quaternion.Z));

	return FVector(Roll, Pitch, Yaw);
}

void AUAV::UpdateForcesAndMoments(float DeltaTime)
{
	// Get the current locations
	CurrentTransform = UAVRigidBody->getGlobalPose();

	CurrentPositionX = UEUnitsToMeters(CurrentTransform.p.x);
	CurrentPositionY = UEUnitsToMeters(CurrentTransform.p.y);
	CurrentPositionZ = UEUnitsToMeters(CurrentTransform.p.z);

	// Get the rotator to get state and transform from world to local coordinates
	FRotator CurrentRotator = FRotator(FQuat(CurrentTransform.q.x, CurrentTransform.q.y, CurrentTransform.q.z, CurrentTransform.q.w));//GetActorRotation();
	FVector EulerRotation = RotatorToEulerInZYX(CurrentRotator);  // Get these in local coords in (Z, Y, X) order - CurrentRotator.Euler() provides (X, Y, Z)

	PxVec3 GlobalLinearVeloicty = UAVRigidBody->getLinearVelocity();
	PxVec3 GlobalAngularVeloicty = UAVRigidBody->getAngularVelocity();
	PxVec3 LocalAngularVelocity = CurrentTransform.q.rotateInv(GlobalAngularVeloicty);

	CurrentRoll = EulerRotation.X;
	CurrentPitch = EulerRotation.Y;
	CurrentYaw = EulerRotation.Z;
		
	CurrentGlobalVelocityZ = UEUnitsToMeters(GlobalLinearVeloicty.z);

	float CurrentRollRate = LocalAngularVelocity.x;
	float CurrentPitchRate = LocalAngularVelocity.y;
	CurrentYawRate = LocalAngularVelocity.z;

	// Convert from [North, West, Up] to [North, East, Down] coordinate frames
	CurrentPitch *= -1;
	CurrentYaw *= -1;
	CurrentPitchRate *= -1;
	CurrentYawRate *= -1;
	
	// Calculate the force and torques from the PID controller
	RollTorqueToApply = RollController.computePIDDirect(DesiredRoll, CurrentRoll, CurrentRollRate, DeltaTime);
	PitchTorqueToApply = PitchController.computePIDDirect(DesiredPitch, CurrentPitch, CurrentPitchRate, DeltaTime);
	YawTorqueToApply = YawController.computePID(DesiredYawRate, CurrentYawRate, DeltaTime);

	float HoverThrust = RootMesh->GetMass() * UEUnitsToMeters(-GWorld->GetGravityZ()) / (cos(DesiredRoll) * cos(DesiredPitch));
	ThrustToApply = AltitudeController.computePIDDirect(DesiredAltitude, CurrentPositionZ, CurrentGlobalVelocityZ, DeltaTime) + HoverThrust;

	// Calculate first-order filter
	float TauRoll =    (RollTorqueToApply > CurrentRollTorque) ? UAV_TAU_UP_ROLL_L : UAV_TAU_DOWN_ROLL_L;
	float TauPitch =   (PitchTorqueToApply > CurrentPitchTorque) ? UAV_TAU_UP_PITCH_M : UAV_TAU_DOWN_PITCH_M;
	float TauYawRate = (YawTorqueToApply > CurrentYawTorque) ? UAV_TAU_UP_YAW_RATE_N : UAV_TAU_DOWN_YAW_RATE_N;
	float TauThrust =  (ThrustToApply > CurrentThrust) ? UAV_TAU_UP_FORCE_F : UAV_TAU_DOWN_FORCE_F;
	float AlphaRoll = DeltaTime / (TauRoll + DeltaTime);
	float AlphaPitch = DeltaTime / (TauPitch + DeltaTime);
	float AlphaYaw = DeltaTime / (TauYawRate + DeltaTime);
	float AlphaThrust = DeltaTime / (TauThrust + DeltaTime);

	RollTorqueToApply = (1 - AlphaRoll) * RollTorqueToApply + AlphaRoll * RollTorqueToApply;
	PitchTorqueToApply = (1 - AlphaPitch) * PitchTorqueToApply + AlphaPitch * PitchTorqueToApply;
	YawTorqueToApply = (1 - AlphaRoll) * YawTorqueToApply + AlphaYaw * YawTorqueToApply;
	ThrustToApply = (1 - AlphaThrust) * ThrustToApply + AlphaThrust * ThrustToApply;

	// Apply the discrete first order filter
	RollTorqueToApply = FMath::Clamp(RollTorqueToApply, -UAV_MAX_ROLL_L, UAV_MAX_ROLL_L);
	PitchTorqueToApply = FMath::Clamp(PitchTorqueToApply, -UAV_MAX_PITCH_M, UAV_MAX_PITCH_M);
	YawTorqueToApply = FMath::Clamp(YawTorqueToApply, -UAV_MAX_YAW_RATE_N, UAV_MAX_YAW_RATE_N);
	ThrustToApply = FMath::Clamp(ThrustToApply, 0.0f, UAV_MAX_FORCE_F);
}

float AUAV::UEUnitsToMeters(float ValueInUnrealUnits) {
	return ValueInUnrealUnits / WorldToMetersRatio;
}

void AUAV::ApplyForces()
{	
	PxVec3 LocalThrust = PxVec3(0, 0, ThrustToApply);
	PxVec3 LocalTorque = PxVec3(RollTorqueToApply, -PitchTorqueToApply, -YawTorqueToApply); // In [North, West, Up] format

	// Convert meters to unreal-unit-newtons
	LocalThrust *= WorldToMetersRatio;
	LocalTorque *= WorldToMetersRatio*WorldToMetersRatio;

	// Apply air resistance
	UAVRigidBody->addForce(-UAV_MU * UAVRigidBody->getLinearVelocity());

	// Rotate to global frame and apply
	UAVRigidBody->addForce(CurrentTransform.q.rotate(LocalThrust));
	UAVRigidBody->addTorque(CurrentTransform.q.rotate(LocalTorque));

	CurrentRollTorque = RollTorqueToApply;
	CurrentPitchTorque = PitchTorqueToApply;
	CurrentYawTorque = YawTorqueToApply;
	CurrentThrust = ThrustToApply;
}
