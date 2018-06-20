#include "Holodeck.h"
#include "NavAgent.h"


// Sets default values
ANavAgent::ANavAgent() {
	PrimaryActorTick.bCanEverTick = true;
}

void ANavAgent::BeginPlay() {
	Super::BeginPlay();
	Target = this->GetActorLocation();
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
