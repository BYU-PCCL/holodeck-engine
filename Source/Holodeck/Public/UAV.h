// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "SimplePID.h"
#include "GameFramework/Pawn.h"
#include "HolodeckUAVController.h"
#include "UAV.generated.h"
UCLASS()
class HOLODECK_API AUAV : public APawn
{
	GENERATED_BODY()
public:
	AUAV();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);
	void UpdateForcesAndMoments(float DeltaTime);
	void ApplyForces();
	float UEUnitsToMeters(float ValueInUnrealUnits);
	FVector RotatorToEulerInZYX(FRotator quat);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredAltitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredYawRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredRoll;
	UPROPERTY(BlueprintReadWrite, Category = UAVMesh)
		UStaticMeshComponent* RootMesh;
	UPROPERTY(BlueprintReadWrite, Category = UAVMesh)
		float ThrustToApply;
private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;
	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;
	// TODO: move these out of the class and to arguments on ApplyForces, rename ApplyForces to ApplyTorquesAndForces
	float RollTorqueToApply;
	float PitchTorqueToApply;
	float YawTorqueToApply;
	// State
	float CurrentThrust;
	float CurrentRollTorque;
	float CurrentPitchTorque;
	float CurrentYawTorque;
	float CurrentPositionX;
	float CurrentPositionY;
	float CurrentPositionZ;
	float CurrentRoll;
	float CurrentPitch;
	float CurrentYaw;
	float CurrentGlobalVelocityZ;
	float CurrentYawRate;
	// Wind
	FVector Wind;
	// Define global constants
	const float Mass = 3.856;
	const float Mu = 1;
	const float MaxRoll = 6.5080;
	const float MaxPitch = 5.087;
	const float MaxYawRate = 0.099828;
	const float MaxForce = 59.844;
	const float TauUpRoll = 0.1904;
	const float TauUpPitch = 0.1904;
	const float TauUpYawRate = 0.1644;
	const float TauUpForce = 0.1644;
	const float TauDownRoll = 0.1904;
	const float TauDownPitch = 0.1904;
	const float TauDownYawRate = 0.2164;
	const float TauDownForce = 0.2164;
	const float RollP = 25.0;
	const float RollI = 0.0;
	const float RollD = 8.0;
	const float PitchP = 25.0;
	const float PitchI = 0.0;
	const float PitchD = 8.0;
	const float YawP = 10.0;
	const float YawI = 0.0;
	const float YawD = 0.0;
	const float AltP = 16.0;
	const float AltI = 5.0;
	const float AltD = 32.0;
};
