// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "UavControlSchemeTargetRollPitch.h"
#include "HolodeckPawnController.h"
#include "UAV.h"

#include "UavController.generated.h"

/**
  * AHolodeckUAVController
  * Controller for the Holodeck UAV
  * Gets the commands to give the the UAV from the action buffer
  * Sets the desired commands on the UAV. 
  */
UCLASS()
class HOLODECK_API AUavController : public AHolodeckPawnController {
	GENERATED_BODY()

public:
	/**
	  * Default Constructor.
	  */
	AUavController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor.
	  */
	~AUavController();

	void AddControlSchemes() {
		ControlSchemes.Add(NewObject<UUavControlSchemeTargetRollPitch>(static_cast<AUav*>(GetPawn())));
	}

private:
	float desiredHeight, currentHeight;
	float desiredRoll, currentRoll;
	float desiredPitch, currentPitch;
	float desiredYawRate, currentYawRate;
};
