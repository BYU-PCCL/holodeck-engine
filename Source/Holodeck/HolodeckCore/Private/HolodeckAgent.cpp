// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"

const char REWARD_KEY[] = "Reward";
const int REWARD_SIZE = 1;
const char TERMINAL_KEY[] = "Terminal";
const int TERMINAL_SIZE = 1;

AHolodeckAgent::AHolodeckAgent() : AgentName("") {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	AddTickPrerequisiteActor(GetController());
	AgentName = "";
}

void AHolodeckAgent::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogHolodeck, Log, TEXT("Initializing HolodeckAgent"));
	HolodeckController = static_cast<AHolodeckPawnController*>(Controller);

	if (HolodeckController == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Couldn't find controller for HolodeckAgent"));
	} else {
		RewardPtr = static_cast<float*>(HolodeckController->Subscribe(AgentName, REWARD_KEY, REWARD_SIZE, sizeof(float)));
		TerminalPtr = static_cast<bool*>(HolodeckController->Subscribe(AgentName, TERMINAL_KEY, TERMINAL_SIZE, sizeof(bool)));
		if (RewardPtr != nullptr)
			*RewardPtr = 0.0;
		if (TerminalPtr != nullptr)
			*TerminalPtr = false;
		HolodeckController->GetActionBuffer(AgentName);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent begin play successful"));	
	}

	//Need to initialize this so that collision events will work (OnActorHit won't be called without it)
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent)){
		PrimitiveComponent->SetNotifyRigidBodyCollision(true);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent collision events enabled"));
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent unable to get UPrimitiveComponent. Collision events disabled."));
	}
}

void AHolodeckAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

bool AHolodeckAgent::Teleport(FVector NewLocation, FRotator NewRotation){
	FHitResult DummyHitResult;
	bool bWasSuccessful = this->K2_SetActorLocationAndRotation(
		NewLocation,
		NewRotation,
		false, //will not be blocked by object in between 
		DummyHitResult, //this object reports the hit result if it can be blocked by objects in between
		false //the object will not retain its momentum.
	);
	if (bWasSuccessful) {
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent teleported successfully"));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent did not teleport successfully"));
	}
	return bWasSuccessful;
}

bool AHolodeckAgent::Teleport(FVector NewLocation){
	FRotator DefaultRotation = this->GetActorRotation();
	return Teleport(NewLocation, DefaultRotation);
}
