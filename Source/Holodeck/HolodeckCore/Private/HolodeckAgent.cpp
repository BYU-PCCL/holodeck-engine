// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"


// Sets default values
AHolodeckAgent::AHolodeckAgent()
{
	PrimaryActorTick.bCanEverTick = true;
	Reward = 0;
	Terminal = false;
	AgentName = "";
}

// Called when the game starts or when spawned
void AHolodeckAgent::BeginPlay()
{
	Super::BeginPlay();
	HolodeckController = Cast<AHolodeckPawnController>(Controller);
	if (HolodeckController == nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("NO Holodeck CONTROLLER ON Holodeck AGENT!"));

	RewardMessage.AgentName = AgentName;
	RewardMessage.Key = "Reward";
	TerminalStateMessage.AgentName = AgentName;
	TerminalStateMessage.Key = "Terminal";

	if (HolodeckController) {
		HolodeckController->Subscribe(RewardMessage, 10);
		HolodeckController->Subscribe(TerminalStateMessage, 1);
	}
}

// Called every frame
void AHolodeckAgent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	RewardMessage.Data = FString::SanitizeFloat(Reward);
	if (HolodeckController)
		HolodeckController->Publish(RewardMessage);

	TerminalStateMessage.Data = Terminal ? TEXT("1") : TEXT("0");
	if (HolodeckController)
		HolodeckController->Publish(TerminalStateMessage);
}

// Called to bind functionality to input
void AHolodeckAgent::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

