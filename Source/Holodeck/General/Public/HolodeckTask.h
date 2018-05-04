// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Actor.h"
#include "HolodeckAgentInterface.h"

#include "HolodeckTask.generated.h"

/**
  * AHolodeckTask
  * A base class for tasks within Holodeck.
  * This class chooses a HolodeckAgent which is trying to perform the task.
  * The task logic then sets the reward and terminal each tick.
  * The child class must remember to call the parent tick class at the end of
  * setting the reward and terminal. This allows the parent class to set those
  * variables in the shared memory.
  */
UCLASS()
class HOLODECK_API AHolodeckTask : public AActor
{
	GENERATED_BODY()
	
public:	
	/**
	  * Default Constructor.
	  */
	AHolodeckTask();

	/**
	  * BeginPlay
	  * Called at the beginning of the game.
	  */
	void BeginPlay() override;
	
	/**
	  * Tick
	  * Called each frame.
	  * @param DeltaSeconds time since last frame.
	  */
	void Tick(float DeltaSeconds) override;

	/**
	  * TaskInit
	  * Starts the task. Should be called by the task master.
	  * Must be overidden by the child task!
	  * @param bOn true if this task should be on.
	  */
	UFUNCTION(BlueprintImplementableEvent)
	void TaskInit(bool bOn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TaskKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Terminal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* MainAgent;
};
