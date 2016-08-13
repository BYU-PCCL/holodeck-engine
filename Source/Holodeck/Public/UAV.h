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
	
	virtual void Tick( float DeltaSeconds ) override;
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

private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;

	// TODO: move these out of the class and to arguments on ApplyForces, rename ApplyForces to ApplyTorquesAndForces
	float ThrustToApply;
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

	// Define global constants
	// TODO: rename l, m, n, f to roll, pitch, yaw_rate, and force
	// TODO: remove UAV_
	// TODO: camel case all of these
	//l is roll
	//m is pitch
	//n is yaw rate
	//f is the force (or fz)
	const float UAV_MASS = 3.856;
	const float UAV_MU = 1;
	const float UAV_MAX_ROLL_L = 6.5080;
	const float UAV_MAX_PITCH_M = 5.087;
	const float UAV_MAX_YAW_RATE_N = 0.099828;
	const float UAV_MAX_FORCE_F = 59.844;
	const float UAV_TAU_UP_ROLL_L = 0.1904;
	const float UAV_TAU_UP_PITCH_M = 0.1904;
	const float UAV_TAU_UP_YAW_RATE_N = 0.1644;
	const float UAV_TAU_UP_FORCE_F = 0.1644;
	const float UAV_TAU_DOWN_ROLL_L = 0.1904;
	const float UAV_TAU_DOWN_PITCH_M = 0.1904;
	const float UAV_TAU_DOWN_YAW_RATE_N = 0.2164;
	const float UAV_TAU_DOWN_FORCE_F = 0.2164;
	const float UAV_ROLL_P = 25.0;
	const float UAV_ROLL_I = 0.0;
	const float UAV_ROLL_D = 8.0;
	const float UAV_PITCH_P = 25.0;
	const float UAV_PITCH_I = 0.0;
	const float UAV_PITCH_D = 8.0;
	const float UAV_YAW_P = 10.0;
	const float UAV_YAW_I = 0.0;
	const float UAV_YAW_D = 0.0;
	const float UAV_ALT_P = 16.0;
	const float UAV_ALT_I = 5.0;
	const float UAV_ALT_D = 32.0;
};
