#include "Holodeck.h"
#include "RotationSensor.h"

URotationSensor::URotationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	SensorName = "RotationSensor";
}

void URotationSensor::InitializeSensor() {
	Super::InitializeSensor();

	//You need to get the pointer to the object the sensor is attached to. 
	Parent = this->GetAttachmentRootActor();
}

void URotationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		FRotator Rotation = Parent->GetActorRotation();
		Rotation = ConvertAngularVector(Rotation, UEToClient);
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Rotation.Roll;
		FloatBuffer[1] = Rotation.Pitch;
		FloatBuffer[2] = Rotation.Yaw;
	}
}
