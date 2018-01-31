#include "Holodeck.h"
#include "RotationSensor.h"

URotationSensor::URotationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void URotationSensor::BeginPlay() {
	Super::BeginPlay();
	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachmentRootActor();
}

void URotationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FRotator Rotation = Parent->GetActorRotation();
		float* FloatBuffer = static_cast<float*>(Buffer);
		Rotation.Roll = FMath::DegreesToRadians(Rotation.Roll);
		Rotation.Pitch = FMath::DegreesToRadians(Rotation.Pitch);
		Rotation.Yaw = FMath::DegreesToRadians(Rotation.Yaw);


		FloatBuffer[0] = Rotation.Roll;
		FloatBuffer[1] = Rotation.Pitch;
		FloatBuffer[2] = Rotation.Yaw;
	}
}
