// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "NavAgent.h"

#include "NavAgentController.generated.h"

/**
* A Holodeck Nav Agent Controller
*/
UCLASS()
class HOLODECK_API ANavAgentController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	ANavAgentController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* Default Destructor
	*/
	~ANavAgentController();

	/**
	* OnPossess
	* Possesses a pawn.
	* @param Pawn the pawn to possess.
	*/
	void OnPossess(APawn* Pawn);

	void AddControlSchemes() override {
		// No Extra Control Schemes
	}
};
