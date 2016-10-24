// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "HoldeckSphereRobotController.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API AHoldeckSphereRobotController : public AHolodeckPawnController
{
	GENERATED_BODY()
	
public:
	AHoldeckSphereRobotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AHoldeckSphereRobotController();

	void Possess(APawn* pawn);
	void OnReceiveCommand(const FHolodeckSphereRobotCommand& Command);
	
};