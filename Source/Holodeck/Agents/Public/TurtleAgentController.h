// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "TurtleAgent.h"

#include "TurtleAgentController.generated.h"

/**
* A Holodeck Sphere Robot Controller
*/
UCLASS()
class HOLODECK_API ATurtleAgentController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	ATurtleAgentController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Default Destructor
	*/
	~ATurtleAgentController();

	/**
	* Possess
	* Possesses a pawn.
	* @param Pawn the pawn to possess.
	*/
	void Possess(APawn* Pawn);

	void AddControlSchemes() override {
		// No control schemes
	}
};
