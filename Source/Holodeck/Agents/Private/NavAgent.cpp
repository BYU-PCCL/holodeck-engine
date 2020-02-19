#include "Holodeck.h"
#include "NavAgent.h"


// Sets default values
ANavAgent::ANavAgent() {
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.NavAgentController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
}

void ANavAgent::InitializeAgent() {
	Super::InitializeAgent();
	Target = this->GetActorLocation();
}

// Called every frame
void ANavAgent::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	float x = FMath::Clamp(CommandArray[0], MIN_DISTANCE, MAX_DISTANCE);
	float y = FMath::Clamp(CommandArray[1], MIN_DISTANCE, MAX_DISTANCE);
	float z = FMath::Clamp(CommandArray[2], MIN_DISTANCE, MAX_DISTANCE);

	Target = FVector(x, y, z);
	Target = ConvertLinearVector(Target, ClientToUE);
}

FVector ANavAgent::GetTarget() {
	return Target;
}

void ANavAgent::SetTarget(float x, float y, float z) {
	Target = FVector(x, y, z);
}
