// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "HolodeckPawnController.h"
#include "HolodeckAgent.generated.h"

UCLASS()
class AHolodeckAgent : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHolodeckAgent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
		FString AgentName;

	void SetReward(int reward) { if (reward_ptr != nullptr) *reward_ptr = reward; };
	void SetTerminal(bool terminal) { if (terminal_ptr != nullptr) *terminal_ptr = terminal; };

private:
	float* reward_ptr;
	bool* terminal_ptr;

	AHolodeckPawnController* HolodeckController;
};
