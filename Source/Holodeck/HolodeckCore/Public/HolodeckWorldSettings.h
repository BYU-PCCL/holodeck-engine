// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "HolodeckWorldSettings.generated.h"

/**
 * The global settings being used by the simulator
 */
UCLASS()
class AHolodeckWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

	float ConstantTimeDeltaBetweenTicks = 0.033;

	float FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds);

public:

	float GetConstantTimeDeltaBetweenTicks();
	void SetConstantTimeDeltaBetweenTicks(float delta);
};
