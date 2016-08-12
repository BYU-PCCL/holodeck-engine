// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "HolodeckWorldSettings.generated.h"

/**
 * The global settings being used by the simulator
 */
UCLASS()
class HOLODECKCORE_API AHolodeckWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

	int AllowedTicksBetweenCommands = -1;
	float ConstantTimeDeltaBetweenTicks = .033;

	float FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds);

public:

	int GetAllowedTicksBetweenCommands();
	void SetAllowedTicksBetweenCommands(int ticks);


	/*
	 * The number of ticks the simulator runs between commands. Set to 0 to disable
	 * pausing entirely and allow the simulator to run
	 */
	float GetConstantTimeDeltaBetweenTicks();
	void SetConstantTimeDeltaBetweenTicks(float delta);
};
