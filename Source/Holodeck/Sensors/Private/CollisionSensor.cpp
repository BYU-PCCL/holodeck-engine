#include "Holodeck.h"
#include "CollisionSensor.h"

UCollisionSensor::UCollisionSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCollisionSensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachmentRootActor();
}

void UCollisionSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		bool IsColliding = Parent->GetVelocity();
		bool* BoolBuffer = static_cast<bool*>(Buffer);
		
	}
}
