// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Actor.h"
#include "HolodeckTask.h"

#include "FollowTask.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API AFollowTask : public AHolodeckTask
{
	GENERATED_BODY()

public:
	AFollowTask();
	~AFollowTask();
};
