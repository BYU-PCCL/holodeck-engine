// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "IMUSensor.h"

UIMUSensor::UIMUSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	SensorName = "IMUSensor";
}

void UIMUSensor::InitializeSensor() {
	Super::InitializeSensor();

	// Cache important variables
	Parent = Cast<UPrimitiveComponent>(this->GetAttachParent());

	World = Parent->GetWorld();
	WorldSettings = World->GetWorldSettings(false, false);
	WorldGravity = WorldSettings->GetGravityZ();

	VelocityThen = FVector();
	VelocityNow = FVector();
	LinearAccelerationVector = FVector();
	AngularVelocityVector = FVector();
}


void UIMUSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		CalculateAccelerationVector(DeltaTime);
		CalculateAngularVelocityVector();

		float* FloatBuffer = static_cast<float*>(Buffer);

		// Convert before sending to user side.
		LinearAccelerationVector = ConvertLinearVector(LinearAccelerationVector, UEToClient);
		AngularVelocityVector = ConvertAngularVector(AngularVelocityVector, NoScale);

		FloatBuffer[0] = LinearAccelerationVector.X;
		FloatBuffer[1] = LinearAccelerationVector.Y;
		FloatBuffer[2] = LinearAccelerationVector.Z;
		FloatBuffer[3] = AngularVelocityVector.X;
		FloatBuffer[4] = AngularVelocityVector.Y;
		FloatBuffer[5] = AngularVelocityVector.Z;
	}
}

void UIMUSensor::CalculateAccelerationVector(float DeltaTime) {
	VelocityThen = VelocityNow;
	VelocityNow = Parent->GetPhysicsAngularVelocityInDegrees();

	RotationNow = this->GetAttachParent()->GetComponentRotation();

	LinearAccelerationVector = VelocityNow - VelocityThen;
	LinearAccelerationVector /= DeltaTime;

	LinearAccelerationVector += FVector(0.0, 0.0, -WorldGravity);

	LinearAccelerationVector = RotationNow.UnrotateVector(LinearAccelerationVector); //changes world axis to local axis
}

void UIMUSensor::CalculateAngularVelocityVector() {
	AngularVelocityVector = Parent->GetPhysicsAngularVelocityInDegrees();

	AngularVelocityVector.X = AngularVelocityVector.X;
	AngularVelocityVector.Y = AngularVelocityVector.Y;
	AngularVelocityVector.Z = AngularVelocityVector.Z;

	AngularVelocityVector = RotationNow.UnrotateVector(AngularVelocityVector); //Rotate from world angles to local angles.

}

FVector UIMUSensor::GetAccelerationVector() {
	return LinearAccelerationVector;
}

FVector UIMUSensor::GetAngularVelocityVector() {
	return AngularVelocityVector;
}
