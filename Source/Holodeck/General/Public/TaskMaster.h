// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Actor.h"
#include "HolodeckTask.h"

#include "TaskMaster.generated.h"

/**
  * ATaskMaster
  * TODO(joshgreaves) : Documentation
  */
UCLASS(Blueprintable)
class HOLODECK_API ATaskMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	  * Default Constructor.
	  */
	ATaskMaster();

	/**
	  * BeginPlay
	  * Called at the beginning of the game.
	  */
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AHolodeckTask*> Tasks;
};
