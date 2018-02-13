// Fill out your copyright notice in the Description page of Project Settings.
#include "Holodeck.h"
#include "UAV.h"
#include <math.h>

const float UAV_MASS = 3.856;
const float UAV_MU = 1;
const float UAV_MAX_ROLL = 6.5080;
const float UAV_MAX_PITCH = 5.087;
const float UAV_MAX_YAW_RATE = .8;
const float UAV_MAX_FORCE = 59.844;
const float UAV_TAU_UP_ROLL = 0.1904;
const float UAV_TAU_UP_PITCH = 0.1904;
const float UAV_TAU_UP_YAW_RATE = 0.04; // 1644;
const float UAV_TAU_UP_FORCE = 0.1644;
const float UAV_TAU_DOWN_ROLL = 0.1904;
const float UAV_TAU_DOWN_PITCH = 0.1904;
const float UAV_TAU_DOWN_YAW_RATE = 0.04;// 0.2164;
const float UAV_TAU_DOWN_FORCE = 0.2164;
const float UAV_ROLL_P = 25.0;
const float UAV_ROLL_I = 0.0;
const float UAV_ROLL_D = 8.0;
const float UAV_PITCH_P = 25.0;
const float UAV_PITCH_I = 0.0;
const float UAV_PITCH_D = 8.0;
const float UAV_YAW_P = 20.0;
const float UAV_YAW_I = 0.0;
const float UAV_YAW_D = 5.0;
const float UAV_ALT_P = 305.0;
const float UAV_ALT_I = 100.0;
const float UAV_ALT_D = 600.0;

AUAV::AUAV() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.HolodeckUAVController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// TODO: the unreal unit conversion should be derived via global settings
	// TODO: the physics substep doesn't seem to have updated velocity/position estimates - we likely need to access the physx body directly (e.g. https://github.com/EpicGames/UnrealEngine/pull/585)
	// TODO: add changes seen in https://answers.unrealengine.com/questions/7459/question-is-120-the-engine-max-frame-rate.html
	// TODO: set deltaTick to 1/40th of a second
	SetActorEnableCollision(true);
	// OnCalculateCustomPhysics.BindUObject(this, &AUAV::SubstepTick);
}

void AUAV::BeginPlay() {
	Super::BeginPlay();

	RootMesh = Cast<UStaticMeshComponent>(RootComponent);

	// Set up the PID Controllers TODO - WHAT TO USE FOR TAU?
	RollController.SetGains(UAV_ROLL_P, UAV_ROLL_I, UAV_ROLL_D);
	PitchController.SetGains(UAV_PITCH_P, UAV_ROLL_I, UAV_ROLL_D);
	YawController.SetGains(UAV_YAW_P, UAV_YAW_I, UAV_PITCH_D);
	AltitudeController.SetGains(UAV_ALT_P, UAV_ALT_I, UAV_ALT_D);
}

void AUAV::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//if (RootMesh) {
	//  RootMesh->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	//}
	UpdateForcesAndMoments(DeltaTime);
	ApplyForces();
}

//void AUAV::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
//{
//  UpdateForcesAndMoments(DeltaTime);
//  ApplyForces();
//}

FVector AUAV::RotatorToEulerInZYX(FRotator Rotator) {
	FQuat Quaternion = Rotator.Quaternion();
	float Roll = atan2(2.0 * (Quaternion.W * Quaternion.X + Quaternion.Y * Quaternion.Z), 1.0 - 2.0 * (Quaternion.X * Quaternion.X + Quaternion.Y * Quaternion.Y));
	float Pitch = asin(2.0 * (Quaternion.W * Quaternion.Y - Quaternion.Z * Quaternion.X));
	float Yaw = atan2(2.0 * (Quaternion.W * Quaternion.Z + Quaternion.X * Quaternion.Y), 1.0 - 2.0 * (Quaternion.Y * Quaternion.Z + Quaternion.Z * Quaternion.Z));
	return FVector(Roll, Pitch, Yaw);
}

void AUAV::UpdateForcesAndMoments(float DeltaTime) {
	GEngine->GameUserSettings->SetScreenResolution(FIntPoint(10, 10)); // TODO - Why is this needed?

	// Get the current locations
	CurrentPositionX = UEUnitsToMeters(GetActorLocation().X);
	CurrentPositionY = UEUnitsToMeters(GetActorLocation().Y);
	CurrentPositionZ = UEUnitsToMeters(GetActorLocation().Z);

	// Get the rotator to get state and transform from world to local coordinates
	FRotator CurrentRotator = GetActorRotation();
	//TODO(mitch): Remove/fix the extraneous conversions. 
	FVector EulerRotation = RotatorToEulerInZYX(CurrentRotator);  // Get these in local coords in (Z, Y, X) order - CurrentRotator.Euler() provides (X, Y, Z)
	FVector CurrentGlobalVelocity = GetVelocity();
	FVector LocalAngularVelocity = CurrentRotator.UnrotateVector(RootMesh->GetPhysicsAngularVelocity());

	CurrentRoll = EulerRotation.X;
	CurrentPitch = EulerRotation.Y;
	CurrentYaw = EulerRotation.Z;
	CurrentGlobalVelocityZ = UEUnitsToMeters(CurrentGlobalVelocity.Z);

	float CurrentRollRate = FMath::DegreesToRadians(LocalAngularVelocity.X);
	float CurrentPitchRate = FMath::DegreesToRadians(LocalAngularVelocity.Y);
	CurrentYawRate = FMath::DegreesToRadians(LocalAngularVelocity.Z);

	// Convert to correct coordinate frames
	CurrentRoll *= -1;
	CurrentPitch *= -1;
	CurrentRollRate *= -1;
	CurrentPitchRate *= -1;

	// Calculate the force and torques from the PID controller
	RollTorqueToApply = RollController.ComputePIDDirect(DesiredRoll, CurrentRoll, CurrentRollRate, DeltaTime);
	PitchTorqueToApply = PitchController.ComputePIDDirect(DesiredPitch, CurrentPitch, CurrentPitchRate, DeltaTime);
	YawTorqueToApply = YawController.ComputePID(DesiredYawRate, CurrentYawRate, DeltaTime);
	float HoverThrust = (RootMesh->GetMass() * -UEUnitsToMeters(GWorld->GetGravityZ())) / (cos(DesiredRoll) * cos(DesiredPitch));
	ThrustToApply = AltitudeController.ComputePIDDirect(DesiredAltitude, CurrentPositionZ, CurrentGlobalVelocityZ, DeltaTime) + HoverThrust;

	// Calculate first-order filter
	float TauRoll = (RollTorqueToApply > CurrentRollTorque) ? UAV_TAU_UP_ROLL : UAV_TAU_DOWN_ROLL;
	float TauPitch = (PitchTorqueToApply > CurrentPitchTorque) ? UAV_TAU_UP_PITCH : UAV_TAU_DOWN_PITCH;
	float TauYawRate = (YawTorqueToApply > CurrentYawTorque) ? UAV_TAU_UP_YAW_RATE : UAV_TAU_DOWN_YAW_RATE;
	float TauThrust = (ThrustToApply > CurrentThrust) ? UAV_TAU_UP_FORCE : UAV_TAU_DOWN_FORCE;
	float AlphaRoll = DeltaTime / (TauRoll + DeltaTime);
	float AlphaPitch = DeltaTime / (TauPitch + DeltaTime);
	float AlphaYaw = DeltaTime / (TauYawRate + DeltaTime);
	float AlphaThrust = DeltaTime / (TauThrust + DeltaTime);
	RollTorqueToApply = (1 - AlphaRoll) * RollTorqueToApply + AlphaRoll * RollTorqueToApply;
	PitchTorqueToApply = (1 - AlphaPitch) * PitchTorqueToApply + AlphaPitch * PitchTorqueToApply;
	YawTorqueToApply = (1 - AlphaYaw) * YawTorqueToApply + AlphaYaw * YawTorqueToApply;
	ThrustToApply = (1 - AlphaThrust) * ThrustToApply + AlphaThrust * ThrustToApply;

	// Calculate Air Resistance
	Wind = -UAV_MU * CurrentGlobalVelocity;

	// Apply the discrete first order filter
	RollTorqueToApply = FMath::Clamp(RollTorqueToApply, -UAV_MAX_ROLL, UAV_MAX_ROLL);
	PitchTorqueToApply = FMath::Clamp(PitchTorqueToApply, -UAV_MAX_PITCH, UAV_MAX_PITCH);
	YawTorqueToApply = FMath::Clamp(YawTorqueToApply, -UAV_MAX_YAW_RATE, UAV_MAX_YAW_RATE);
	ThrustToApply = FMath::Clamp(ThrustToApply, -UAV_MAX_FORCE, UAV_MAX_FORCE);
}

float AUAV::UEUnitsToMeters(float ValueInUnrealUnits) {
	return ValueInUnrealUnits / 100.0;
}

void AUAV::ApplyForces() {
	FVector LocalThrust = FVector(0, 0, ThrustToApply);
	FVector LocalTorque = FVector(-RollTorqueToApply, -PitchTorqueToApply, YawTorqueToApply); //Make sure to apply the forces how unreal expects it
																							  // Convert meters to unreal-unit-newtons
	LocalThrust *= 100;
	LocalTorque *= 10000;
	// Scale force for substepping
	//LocalThrust /= 16;
	//LocalTorque /= 16;
	//Wind /= 16
	// Apply torques and forces in global coordinates
	RootMesh->AddTorque(GetActorRotation().RotateVector(LocalTorque));
	RootMesh->AddForce(GetActorRotation().RotateVector(LocalThrust));
	RootMesh->AddForce(Wind);
	CurrentRollTorque = RollTorqueToApply;
	CurrentPitchTorque = PitchTorqueToApply;
	CurrentYawTorque = YawTorqueToApply;
	CurrentThrust = ThrustToApply;
}
