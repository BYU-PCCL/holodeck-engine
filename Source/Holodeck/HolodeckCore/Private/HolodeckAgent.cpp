// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAgent.h"

const char REWARD_KEY[] = "Reward";
const int REWARD_SIZE = 1;
const char TERMINAL_KEY[] = "Terminal";
const int TERMINAL_SIZE = 1;

AHolodeckAgent::AHolodeckAgent() : AgentName("") {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics; //The tick function will we called before any physics simulation. 
	AddTickPrerequisiteActor(GetController()); //The agent's controller will always tick before the agent. 
	AgentName = "";
}

void AHolodeckAgent::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogHolodeck, Log, TEXT("Initializing HolodeckAgent"));
	HolodeckController = static_cast<AHolodeckPawnController*>(Controller);

	if (HolodeckController == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Couldn't find controller for HolodeckAgent"));
	} else {
		//We found the controller, so tell it to set up the action buffers. Also, open up the channels for the reward pointer and terminal pointer.
		RewardPtr = static_cast<float*>(HolodeckController->Subscribe(AgentName, REWARD_KEY, REWARD_SIZE, sizeof(float)));
		TerminalPtr = static_cast<bool*>(HolodeckController->Subscribe(AgentName, TERMINAL_KEY, TERMINAL_SIZE, sizeof(bool)));
		if (RewardPtr != nullptr)
			*RewardPtr = 0.0;
		if (TerminalPtr != nullptr)
			*TerminalPtr = false;
		HolodeckController->GetActionBuffer(AgentName);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent begin play successful"));
	}

	//These functions are used in conjunction to give all of the parameters of agents to the python binding. 
	//It is important that these are called in this order. Each rely on the successful completion of the prior. 
	if (bShouldExposeSettings) {
		GetServer();
		GetSettingsBuffer();
		UploadSettings(); //This is essentially a pure virtual function. It is fine here because BeginPlay() is called after the constructor is called.
	}


	//Need to initialize this so that collision events will work (OnActorHit won't be called without it)
	//This is needed specifically for the collision sensor.
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

bool AHolodeckAgent::Teleport(const FVector& NewLocation, FRotator NewRotation){
	FHitResult DummyHitResult;
	bool bWasSuccessful = this->K2_SetActorLocationAndRotation(
		NewLocation,
		NewRotation,
		false, //will not be blocked by object in between current and new location. 
		DummyHitResult, //this object is where the hit result is reported, if teleport can be blocked by objects in between.
		false //the object will not retain its momentum.
	);
	if (bWasSuccessful) {
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent teleported successfully"));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent did not teleport successfully"));
	}
	return bWasSuccessful;
}

bool AHolodeckAgent::Teleport(const FVector& NewLocation){
	FRotator DefaultRotation = this->GetActorRotation();
	return Teleport(NewLocation, DefaultRotation);
}

void AHolodeckAgent::GetSettingsBuffer() {
	FString SettingName = AgentName + "_Settings";
	if (Server != nullptr) {
		SettingsBuffer = static_cast<float*>(Server->SubscribeSetting(TCHAR_TO_UTF8(*SettingName), this->GetNumSettings() * sizeof(float)));
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent::GetSettingsBuffer failed due to null Server pointer"));
	}
}

void AHolodeckAgent::GetServer() {
	if (Server != nullptr) return;
	UHolodeckGameInstance* Instance;
	if (this->IsControlled()) {
		Instance = static_cast<UHolodeckGameInstance*>(this->GetController()->GetGameInstance());
		if (Instance != nullptr)
			Server = Instance->GetServer();
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("Game Instance is not UHolodeckGameInstance."));
	 }
}
