// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckAgent.h"

#include "SphereRobot.generated.h"

UCLASS()
class HOLODECK_API ASphereRobot : public AHolodeckAgent
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor.
	  */
	ASphereRobot();
	
	/**
	  * Tick
	  * Called each frame.
	  * @param DeltaSeconds the time since the last tick.
	  */
	void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForwardSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Counter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasWon;
};
