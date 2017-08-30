// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "SphereRobot.h"

#include "HolodeckSphereRobotController.generated.h"

/**
 * A Holodeck Sphere Robot Controller
 */
UCLASS()
class HOLODECK_API AHolodeckSphereRobotController : public AHolodeckPawnController
{
	GENERATED_BODY()
	
public:
	/**
	  * Default Constructor
	  */
	AHolodeckSphereRobotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor
	  */
	~AHolodeckSphereRobotController();

	/**
	  * Possess
	  * Possesses a pawn.
	  * @param Pawn the pawn to possess.
	  */
	void Possess(APawn* Pawn);

	/**
	  * ExecuteCommand
	  * Executes the commands issued from the client.
	  */
	void ExecuteCommand() override;

protected:
	int GetActionSpaceDimension() override { return 3; };
};
