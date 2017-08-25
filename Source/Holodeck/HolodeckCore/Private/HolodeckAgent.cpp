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
	if (HolodeckController == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("No Holodeck Controller on Holodeck Agent!"));

	if (HolodeckController) {
		reward_ptr = HolodeckController->Subscribe(AgentName, "Reward", sizeof(float));
		terminal_ptr = HolodeckController->Subscribe(AgentName, "Terminal", sizeof(float));
		*reward_ptr = 0.0;
		*terminal_ptr = 0.0;
	}
}

// Called every frame
void AHolodeckAgent::Tick( float DeltaTime )
{
	*reward_ptr = (float)(((int)(*reward_ptr) + 1) % 100);
	// Nothing needed...
}

// Called to bind functionality to input
void AHolodeckAgent::SetupPlayerInputComponent(class UInputComponent* component)
{
	Super::SetupPlayerInputComponent(component);
}

