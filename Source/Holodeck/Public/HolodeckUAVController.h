// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "HolodeckUAVController.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API AHolodeckUAVController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	AHolodeckUAVController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AHolodeckUAVController();

	void Possess(APawn* Pawn);

	void OnReceiveCommand(const FHolodeckUAVCommand& Command);
	void OnReceiveConfiguration(const FHolodeckUAVConfiguration& Configuration);

	void tick(float DeltaTime);

private:
	float desiredHeight, currentHeight;
	float desiredRoll, currentRoll;
	float desiredPitch, currentPitch;
	float desiredYawRate, currentYawRate;
	
	// Define global constants
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
