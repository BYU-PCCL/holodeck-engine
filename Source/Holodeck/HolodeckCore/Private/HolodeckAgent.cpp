// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"


// Sets default values
AHolodeckAgent::AHolodeckAgent()
{
	PrimaryActorTick.bCanEverTick = true;
	AgentName = "";
}

// Called when the game starts or when spawned
void AHolodeckAgent::BeginPlay()
{
	Super::BeginPlay();
	HolodeckController = Cast<AHolodeckPawnController>(Controller);
	if (HolodeckController == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("No Holodeck Controller on Holodeck Agent!"));
	} else {
		reward_ptr = static_cast<float*>(HolodeckController->Subscribe(AgentName, "Reward", 1, sizeof(float)));
		terminal_ptr = static_cast<bool*>(HolodeckController->Subscribe(AgentName, "Terminal", 1, sizeof(bool)));
		if (reward_ptr != nullptr)
			*reward_ptr = 0.0;
		if (terminal_ptr != nullptr)
			*terminal_ptr = false;
	}
}

// Called every frame
void AHolodeckAgent::Tick( float DeltaTime )
{
}

// Called to bind functionality to input
void AHolodeckAgent::SetupPlayerInputComponent(class UInputComponent* component)
{
	Super::SetupPlayerInputComponent(component);
}

