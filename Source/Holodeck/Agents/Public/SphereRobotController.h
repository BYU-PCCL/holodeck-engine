// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "SphereRobot.h"

#include "SphereRobotController.generated.h"

/**
 * A Holodeck Sphere Robot Controller
 */
UCLASS()
class HOLODECK_API ASphereRobotController : public AHolodeckPawnController
{
	GENERATED_BODY()
	
public:
	/**
	  * Default Constructor
	  */
	ASphereRobotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor
	  */
	~ASphereRobotController();

	/**
	  * OnPossess
	  * Possesses a pawn.
	  * @param Pawn the pawn to possess.
	  */
	void OnPossess(APawn* Pawn);

	void AddControlSchemes() override {
		// No control schemes
	}
};
