#include "Holodeck.h"
#include "CollisionSensor.h"

UCollisionSensor::UCollisionSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCollisionSensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetOwner();

	//Set the hit delegate. Give it to the parent. 
	FScriptDelegate HitDelegate;
	HitDelegate.BindUFunction(this, TEXT("OnHit"));
	this->GetAttachmentRootActor()->OnActorHit.AddUnique(HitDelegate);
	
}

void UCollisionSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		bool* BoolBuffer = static_cast<bool*>(Buffer);
		BoolBuffer[0] = bIsColliding;
	}
	//bIsColliding = false;
}

void UCollisionSensor::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	bIsColliding = true;
	UE_LOG(LogHolodeck, Log, TEXT("UCollisionSensor::OnHit was called. Hit detected!"));
}
