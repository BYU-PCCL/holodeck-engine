// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "UAVControlSchemeTargetRollPitch.h"
#include "HolodeckPawnController.h"
#include "UAV.h"

#include "HolodeckUAVController.generated.h"

/**
  * AHolodeckUAVController
  * Controller for the Holodeck UAV
  * Gets the commands to give the the UAV from the action buffer
  * Sets the desired commands on the UAV. 
  */
UCLASS()
class HOLODECK_API AHolodeckUAVController : public AHolodeckPawnController {
	GENERATED_BODY()

public:
	/**
	  * Default Constructor.
	  */
	AHolodeckUAVController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor.
	  */
	~AHolodeckUAVController();

	void AddControlSchemes() {
		ControlSchemes.Add(NewObject<UUAVControlSchemeTargetRollPitch>(static_cast<AUAV*>(GetPawn())));
	}

private:
	float desiredHeight, currentHeight;
	float desiredRoll, currentRoll;
	float desiredPitch, currentPitch;
	float desiredYawRate, currentYawRate;
};
