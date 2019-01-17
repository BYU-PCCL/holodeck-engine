// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Actor.h"
#include "HolodeckTask.h"

#include "DistanceTask.generated.h"

/**
*
*/
UCLASS()
class HOLODECK_API ADistanceTask : public AHolodeckTask
{
	GENERATED_BODY()

public:
	ADistanceTask();
	~ADistanceTask();
};
