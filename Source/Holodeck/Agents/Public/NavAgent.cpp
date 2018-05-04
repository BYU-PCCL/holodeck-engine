#include "Holodeck.h"
#include "NavAgent.h"

// Sets default values
ANavAgent::ANavAgent() {
	PrimaryActorTick.bCanEverTick = true;
}

void ANavAgent::BeginPlay() {
	Super::BeginPlay();
	Target = this->GetActorLocation();

	UE_LOG(LogHolodeck, Log, TEXT("Initializing HolodeckAgent"));
	if (InitializeController())
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent BeginPlay was successful"));
	//Need to initialize this so that collision events will work (OnActorHit won't be called without it)
	//This is needed specifically for the collision sensor.
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent)) {
		PrimitiveComponent->SetNotifyRigidBodyCollision(true);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent collision events enabled"));
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("HolodeckAgent unable to get UPrimitiveComponent. Collision events disabled."));
	}
}

// Called every frame
void ANavAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

FVector ANavAgent::GetTarget() {
	return Target;
}

void ANavAgent::SetTarget(float x, float y, float z) {
	Target = FVector(x, y, z);
}

bool ANavAgent::Teleport_Implementation(const FVector& NewLocation) {
	FRotator DefaultRotation = this->GetActorRotation();
	return TeleportAndRotate(NewLocation, DefaultRotation);
}

bool ANavAgent::TeleportAndRotate_Implementation(const FVector& NewLocation, FRotator NewRotation) {
	FHitResult DummyHitResult;
	return this->K2_SetActorLocationAndRotation(
		NewLocation,
		NewRotation,
		false, //will not be blocked by object in between current and new location. 
		DummyHitResult, //this object is where the hit result is reported, if teleport can be blocked by objects in between.
		false //the object will not retain its momentum.
	);
}

bool ANavAgent::InitializeController_Implementation() {
	PawnController = static_cast<AHolodeckPawnController*>(Controller);

	if (PawnController == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("Couldn't find controller for HolodeckAgent"));
		return false;
	} else {
		//We found the controller, so tell it to set up the action buffers. Also, open up the channels for the reward pointer and terminal pointer.
		RewardPtr = static_cast<float*>(PawnController->Subscribe(AgentName, REWARD_KEY, REWARD_SIZE, sizeof(float)));
		TerminalPtr = static_cast<bool*>(PawnController->Subscribe(AgentName, TERMINAL_KEY, TERMINAL_SIZE, sizeof(bool)));
		if (RewardPtr != nullptr)
			*RewardPtr = 0.0;
		if (TerminalPtr != nullptr)
			*TerminalPtr = false;
		PawnController->GetActionBuffer(AgentName);
		UE_LOG(LogHolodeck, Log, TEXT("HolodeckAgent controller setup was successful"));
		return true;
	}
}

FString ANavAgent::GetAgentName_Implementation() {
	return AgentName;
}

bool ANavAgent::SetAgentName_Implementation(const FString& Name) {
	AgentName = Name;
	return true;
}

bool ANavAgent::SpawnController_Implementation() {
	SpawnDefaultController();
	return true;
}

AHolodeckPawnController* ANavAgent::GetHolodeckPawnController_Implementation() {
	return PawnController;
}

bool ANavAgent::SetHolodeckPawnController_Implementation(AHolodeckPawnController* HolodeckController) {
	PawnController = HolodeckController;
	return true;
}

bool ANavAgent::SetTerminal_Implementation(bool Terminal) {
	if (TerminalPtr != nullptr) {
		*TerminalPtr = Terminal;
		return true;
	} else {
		return false;
	}
}

bool ANavAgent::SetReward_Implementation(int Reward) {
	if (RewardPtr != nullptr) {
		*RewardPtr = Reward;
		return true;
	} else {
		return false;
	}
}