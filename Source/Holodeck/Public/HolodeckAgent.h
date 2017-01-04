// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "HolodeckPawnController.h"
#include "HolodeckAgent.generated.h"

UCLASS()
class HOLODECK_API AHolodeckAgent : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHolodeckAgent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	int Score;

	UPROPERTY(BlueprintReadWrite)
	bool Terminal;

	AHolodeckPawnController* HolodeckController;
	FHolodeckSensorData RewardMessage;
	FHolodeckSensorData TerminalStateMessage;

};
