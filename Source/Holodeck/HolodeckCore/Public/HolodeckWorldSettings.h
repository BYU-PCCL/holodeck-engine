// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "HolodeckWorldSettings.generated.h"

/**
 * The global settings being used by the simulator.
 * This is where the tick time is set.
 */
UCLASS()
class AHolodeckWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	/**
	  * FixupDeltaSeconds
	  * Overidden to ensure a tick is 0.033 seconds.
	  */
	float FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds) override;

	/**
	  * GetConstantTimeDeltaBetweenTicks
	  * Gets the time between ticks.
	  * @return the time between ticks in seconds.
	  */
	float GetConstantTimeDeltaBetweenTicks();

	/**
	  * SetConstantTimeDeltaBetweenTicks(float Delta)
	  * Sets the time of one tick.
	  * @param Delta the time for one tick.
	  */
	void SetConstantTimeDeltaBetweenTicks(float Delta);

private:
	float ConstantTimeDeltaBetweenTicks = 0.033;
};
