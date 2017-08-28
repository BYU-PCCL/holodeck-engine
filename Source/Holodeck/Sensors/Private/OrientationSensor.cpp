// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "OrientationSensor.h"


// Sets default values for this component's properties
UOrientationSensor::UOrientationSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOrientationSensor::BeginPlay() {
	Super::BeginPlay();

	// Set the controller, parent, and root mesh
	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);
	Parent = Cast<UPrimitiveComponent>(this->GetAttachParent());
	RootMesh = Cast<UStaticMeshComponent>(this->GetAttachParent());

	World = Parent->GetWorld();
}

FString UOrientationSensor::GetDataKey() {
	return "OrientationSensor";
}

int UOrientationSensor::GetNumItems() {
	return 9;
}

int UOrientationSensor::GetItemSize() {
	return sizeof(float);
}

// Called every frame
void UOrientationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != NULL) {
		if (RootMesh != NULL) {
			FVector Forward = RootMesh->GetForwardVector();
			FVector Right = RootMesh->GetRightVector();
			FVector Up = RootMesh->GetUpVector();

			float* float_buffer = static_cast<float*>(buffer);

			float_buffer[0] = Forward.X;
			float_buffer[1] = Forward.Y;
			float_buffer[2] = Forward.Z;
			float_buffer[3] = Right.X;
			float_buffer[4] = Right.Y;
			float_buffer[5] = Right.Z;
			float_buffer[6] = Up.X;
			float_buffer[7] = Up.Y;
			float_buffer[8] = Up.Z;
		}
	}
}
