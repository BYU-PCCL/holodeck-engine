#include "Holodeck.h"
#include "CollisionSensor.h"

UCollisionSensor::UCollisionSensor() {
	UE_LOG(LogHolodeck, Log, TEXT("Constructor was called"));
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UCollisionSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	UE_LOG(LogHolodeck, Log, TEXT("It ticks."));
	if (Parent != nullptr && bOn) {
		bool* BoolBuffer = static_cast<bool*>(Buffer);
		BoolBuffer[0] = bIsColliding;
	}
	bIsColliding = false;
}

void UCollisionSensor::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	bIsColliding = true;
}
 
void UCollisionSensor::InitializeComponent() {
	Super::InitializeComponent();
	Parent = this->GetOwner();
	//Set the hit delegate. Give it to the parent. 
	FScriptDelegate HitDelegate;
	HitDelegate.BindUFunction(this, TEXT("OnHit"));
	if (Parent) {
		Parent->OnActorHit.AddUnique(HitDelegate);
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("UCollisionSensor::Parent was never initialized. Cannot add HitDelegate"));
	}
}
