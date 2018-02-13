// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HolodeckCore/Public/HolodeckAgent.h"
#include "Holodeck.h"
#include "HolodeckAgent.h"

#include "NavAgent.generated.h"

UCLASS()
class HOLODECK_API ANavAgent : public AHolodeckAgent
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor.
	*/
	ANavAgent();

	/**
	* BeginPlay
	* Called when the game starts.
	* Registers the reward and terminal signals.
	*/
	void BeginPlay() override;

	/**
	* Tick
	* Called each frame.
	* @param DeltaSeconds the time since the last tick.
	*/
	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Holodeck")
	FVector GetTarget();

	UFUNCTION(BlueprintCallable, Category = "Holodeck")
	void SetTarget(float x, float y, float z);

private:
	FVector Target;

};
