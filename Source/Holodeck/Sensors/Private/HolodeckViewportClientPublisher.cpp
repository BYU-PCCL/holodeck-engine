// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClientPublisher.h"


// Sets default values for this component's properties
UHolodeckViewportClientPublisher::UHolodeckViewportClientPublisher() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHolodeckViewportClientPublisher::BeginPlay() {
	Super::BeginPlay();

	ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient && bOn) {
		ViewportClient->SetBuffer(buffer);
	}
}


// Called every frame
void UHolodeckViewportClientPublisher::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
}

int UHolodeckViewportClientPublisher::GetNumItems() {
	return 512 * 512 * 4;
	if (ViewportClient)
	{
		FVector2D Dims;
		ViewportClient->GetViewportSize(Dims);
		return Dims.X * Dims.Y * 3; // X by Y pixels, 3 for RGB
	}
	else {
		return 0;
	}
}

int UHolodeckViewportClientPublisher::GetItemSize() {
	return sizeof(FColor);
}
