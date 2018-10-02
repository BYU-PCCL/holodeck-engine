// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"

const char REWARD_KEY[] = "Reward";
const int REWARD_SIZE = 1;
const char TERMINAL_KEY[] = "Terminal";
const int TERMINAL_SIZE = 1;

AHolodeckAgent::AHolodeckAgent() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics; //The tick function will we called before any physics simulation. 
	AddTickPrerequisiteActor(GetController()); //The agent's controller will always tick before the agent.
}

void AHolodeckAgent::BeginPlay() {
	UE_LOG(LogHolodeck, Log, TEXT("Initializing HolodeckAgent"));
	Super::BeginPlay();

	if(!InitializeController())
		UE_LOG(LogHolodeck, Warning, TEXT("Couldn't initialize HolodeckPawnController for HolodeckAgent."));

	//Need to initialize this so that collision events will work (OnActorHit won't be called without it)
	//This is needed specifically for the collision sensor.
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent)) {
		PrimitiveComponent->SetNotifyRigidBodyCollision(true);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent collision events enabled"));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent unable to get UPrimitiveComponent. Collision events disabled."));
	}
}

void AHolodeckAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AHolodeckAgent::SetReward(float Reward) {
	if (RewardPtr != nullptr)
		*RewardPtr = Reward;
	else
		UE_LOG(LogHolodeck, Warning, TEXT("Trying to set reward for HolodeckAgent without a HolodeckController"));
}

void AHolodeckAgent::SetTerminal(bool bTerminal) {
	if (TerminalPtr != nullptr)
		*TerminalPtr = bTerminal;
	else
		UE_LOG(LogHolodeck, Warning, TEXT("Trying to set terminal for HolodeckAgent without a HolodeckController"));
}

bool AHolodeckAgent::Teleport(const FVector& NewLocation, const FRotator& NewRotation) {
	UE_LOG(LogHolodeck, Log, TEXT("Attempting to teleport HolodeckAgent %s"), *AgentName);
	FHitResult DummyHitResult;
	bool bWasSuccessful = this->K2_SetActorLocationAndRotation(
		NewLocation,
		NewRotation,
		false, //will not be blocked by object in between current and new location. 
		DummyHitResult, //this object is where the hit result is reported, if teleport can be blocked by objects in between.
		false //the object will not retain its momentum.
	);

	if (bWasSuccessful) {
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent %s teleported successfully"), *AgentName);
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent %s did not teleport successfully"), *AgentName);
	}
	
	return bWasSuccessful;
}

bool AHolodeckAgent::Teleport(const FVector& NewLocation){
	FRotator DefaultRotation = this->GetActorRotation();
	return Teleport(NewLocation, DefaultRotation);
}

bool AHolodeckAgent::InitializeController() {
	UE_LOG(LogHolodeck, Log, TEXT("Attempting to initialize controller for HolodeckAgent"));

	if (AgentName == "") {
		UE_LOG(LogHolodeck, Error, TEXT("HolodeckAgent doesn't have a name! Must set HolodeckAgent's name"));
		return false;
	}

	HolodeckController = static_cast<AHolodeckPawnController*>(Controller);

	if (HolodeckController == nullptr) {
		UE_LOG(LogHolodeck, Error, TEXT("Couldn't find a HolodeckPawnController for HolodeckAgent"));
		return false;
	} else {
		// We found the controller, so tell it to set up the action buffers.
		// Also, open up the channels for the reward pointer and terminal pointer.
		RewardPtr = static_cast<float*>(HolodeckController->Subscribe(AgentName, REWARD_KEY, REWARD_SIZE, sizeof(float)));
		TerminalPtr = static_cast<bool*>(HolodeckController->Subscribe(AgentName, TERMINAL_KEY, TERMINAL_SIZE, sizeof(bool)));
		if (RewardPtr != nullptr)
			*RewardPtr = 0.0;
		if (TerminalPtr != nullptr)
			*TerminalPtr = false;
		HolodeckController->AllocateBuffers(AgentName);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent controller setup was successful"));
		return true;
	}
}
