// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "OrientationSensor.h"


// Sets default values for this component's properties
UOrientationSensor::UOrientationSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOrientationSensor::BeginPlay()
{
	Super::BeginPlay();

	// Set the controller, parent, and root mesh
	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);
	Parent = Cast<UPrimitiveComponent>(this->GetAttachParent());
	RootMesh = Cast<UStaticMeshComponent>(this->GetAttachParent());

	World = Parent->GetWorld();
}

void UOrientationSensor::SetDataType()
{
	ResultData.Type = "OrientationSensor";
}

// Called every frame
void UOrientationSensor::TickSensorComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	if (Parent != NULL) {
		
		if (RootMesh != NULL) {
			FVector Forward = RootMesh->GetForwardVector();
			FVector Right = RootMesh->GetRightVector();
			FVector Up = RootMesh->GetUpVector();

			ResultData.Data =
				FString::SanitizeFloat(Forward.X) + "," +
				FString::SanitizeFloat(Forward.Y) + "," +
				FString::SanitizeFloat(Forward.Z) + "," +
				FString::SanitizeFloat(Right.X) + "," +
				FString::SanitizeFloat(Right.Y) + "," +
				FString::SanitizeFloat(Right.Z) + "," +
				FString::SanitizeFloat(Up.X) + "," +
				FString::SanitizeFloat(Up.Y) + "," +
				FString::SanitizeFloat(Up.Z);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: Failed to cast 'this->GetAttachParent()' to UPrimitiveComponent"));
	}
}

