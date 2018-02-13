// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "NavAgent.h"

#include "HolodeckNavAgentController.generated.h"

/**
* A Holodeck Nav Agent Controller
*/
UCLASS()
class HOLODECK_API AHolodeckNavAgentController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	AHolodeckNavAgentController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Default Destructor
	*/
	~AHolodeckNavAgentController();

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
