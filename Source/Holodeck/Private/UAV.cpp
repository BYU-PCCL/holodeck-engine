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
	// TODO: the unreal unit conversion should be derived via global settings
	// TODO: the physics substep doesn't seem to have updated velocity/position estimates - we likely need to access the physx body directly (e.g. https://github.com/EpicGames/UnrealEngine/pull/585)
	// TODO: accept desired roll/pitch/yaw/altitude commands via HolodeckMessage
	// TODO: add changes seen in https://answers.unrealengine.com/questions/7459/question-is-120-the-engine-max-frame-rate.html
	// TODO: set deltaTick to 1/40th of a second
	SetActorEnableCollision(true);
	// OnCalculateCustomPhysics.BindUObject(this, &AUAV::SubstepTick);
}
// Called when the game starts or when spawned
void AUAV::BeginPlay()
{
	Super::BeginPlay();

	RootMesh = Cast<UStaticMeshComponent>(RootComponent);
	// Set up the PID Controllers TODO - WHAT TO USE FOR TAU?
	RollController.setGains(RollP, RollI, RollD, TauDownRoll);
	PitchController.setGains(PitchP, RollI, RollD, TauDownPitch);
	YawController.setGains(YawP, YawI, PitchD, TauDownYawRate);
	AltitudeController.setGains(AltP, AltI, AltD, TauDownForce);
}
// Called every frame
void AUAV::Tick(float DeltaTime)
{
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
void AUAV::UpdateForcesAndMoments(float DeltaTime)
{
	GEngine->GameUserSettings->SetScreenResolution(FIntPoint(10, 10));
	// Get the current locations
	CurrentPositionX = UEUnitsToMeters(GetActorLocation().X);
	CurrentPositionY = UEUnitsToMeters(GetActorLocation().Y);
	CurrentPositionZ = UEUnitsToMeters(GetActorLocation().Z);
	// Get the rotator to get state and transform from world to local coordinates
	FRotator CurrentRotator = GetActorRotation();
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
	// Convert from [North, West, Up] to [North, East, Down] coordinate frames
	CurrentPitch *= -1;
	CurrentYaw *= -1;
	CurrentPitchRate *= -1;
	CurrentYawRate *= -1;
	// Calculate the force and torques from the PID controller
	RollTorqueToApply = RollController.computePIDDirect(DesiredRoll, CurrentRoll, CurrentRollRate, DeltaTime);
	PitchTorqueToApply = PitchController.computePIDDirect(DesiredPitch, CurrentPitch, CurrentPitchRate, DeltaTime);
	YawTorqueToApply = YawController.computePID(DesiredYawRate, CurrentYawRate, DeltaTime);
	float HoverThrust = (RootMesh->GetMass() * -UEUnitsToMeters(GWorld->GetGravityZ())) / (cos(DesiredRoll) * cos(DesiredPitch));
	ThrustToApply = AltitudeController.computePIDDirect(DesiredAltitude, CurrentPositionZ, CurrentGlobalVelocityZ, DeltaTime) + HoverThrust;
	// Calculate first-order filter
	float TauRoll = (RollTorqueToApply > CurrentRollTorque) ? TauUpRoll : TauDownRoll;
	float TauPitch = (PitchTorqueToApply > CurrentPitchTorque) ? TauUpPitch : TauDownPitch;
	float TauYawRate = (YawTorqueToApply > CurrentYawTorque) ? TauUpYawRate : TauDownYawRate;
	float TauThrust = (ThrustToApply > CurrentThrust) ? TauUpForce : TauDownForce;
	float AlphaRoll = DeltaTime / (TauRoll + DeltaTime);
	float AlphaPitch = DeltaTime / (TauPitch + DeltaTime);
	float AlphaYaw = DeltaTime / (TauYawRate + DeltaTime);
	float AlphaThrust = DeltaTime / (TauThrust + DeltaTime);
	RollTorqueToApply = (1 - AlphaRoll) * RollTorqueToApply + AlphaRoll * RollTorqueToApply;
	PitchTorqueToApply = (1 - AlphaPitch) * PitchTorqueToApply + AlphaPitch * PitchTorqueToApply;
	YawTorqueToApply = (1 - AlphaRoll) * YawTorqueToApply + AlphaYaw * YawTorqueToApply;
	ThrustToApply = (1 - AlphaThrust) * ThrustToApply + AlphaThrust * ThrustToApply;
	// Calculate Air Resistance
	Wind = -Mu * CurrentGlobalVelocity;
	// Apply the discrete first order filter
	RollTorqueToApply = FMath::Clamp(RollTorqueToApply, -MaxRoll, MaxRoll);
	PitchTorqueToApply = FMath::Clamp(PitchTorqueToApply, -MaxPitch, MaxPitch);
	YawTorqueToApply = FMath::Clamp(YawTorqueToApply, -MaxYawRate, MaxYawRate);
	ThrustToApply = FMath::Clamp(ThrustToApply, -MaxForce, MaxForce);
}
float AUAV::UEUnitsToMeters(float ValueInUnrealUnits) {
	return ValueInUnrealUnits / 100.0;
}
void AUAV::ApplyForces()
{
	FVector LocalThrust = FVector(0, 0, ThrustToApply);
	FVector LocalTorque = FVector(RollTorqueToApply, -PitchTorqueToApply, -YawTorqueToApply); // In [North, West, Up] format
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Wind.ToString());/*
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wind: %f, Velocity: %f"), Wind.ToString(), GetVelocity().ToString()));*/
}
