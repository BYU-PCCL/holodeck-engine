// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"

const char REWARD_KEY[] = "Reward";
const int REWARD_SIZE = 1;
const char TERMINAL_KEY[] = "Terminal";
const int TERMINAL_SIZE = 1;

AHolodeckAgent::AHolodeckAgent() : AgentName("") {
	PrimaryActorTick.bCanEverTick = true;
	AgentName = "";
}

void AHolodeckAgent::BeginPlay() {
	Super::BeginPlay();
	HolodeckController = Cast<AHolodeckPawnController>(Controller);

	if (HolodeckController == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("No Holodeck Controller on Holodeck Agent!"));
	} else {
		RewardPtr = static_cast<float*>(HolodeckController->Subscribe(AgentName, REWARD_KEY, REWARD_SIZE, sizeof(float)));
		TerminalPtr = static_cast<bool*>(HolodeckController->Subscribe(AgentName, TERMINAL_KEY, TERMINAL_SIZE, sizeof(bool)));
		if (RewardPtr != nullptr)
			*RewardPtr = 0.0;
		if (TerminalPtr != nullptr)
			*TerminalPtr = false;
		HolodeckController->GetActionBuffer(AgentName);
	}
}

void AHolodeckAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (HolodeckController != nullptr)
		HolodeckController->ExecuteCommand();
}
