// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClientPublisher.h"

// Sets default values for this component's properties
UHolodeckViewportClientPublisher::UHolodeckViewportClientPublisher() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHolodeckViewportClientPublisher::BeginPlay() {
	Super::BeginPlay();

	ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient && bOn) {
		ViewportClient->SetBuffer(Buffer);
	}
}

void UHolodeckViewportClientPublisher::TickSensorComponent(float DeltaTime,
														   ELevelTick TickType,
														   FActorComponentTickFunction* ThisTickFunction) {
	// The pixel data is captured on the rendering thread.
	// All this class needs to do is set the buffer at begin play.
}

int UHolodeckViewportClientPublisher::GetNumItems() {
	if (ViewportClient) {
		FVector2D Dims;
		ViewportClient->GetViewportSize(Dims);
		return Dims.X * Dims.Y * 4; // X by Y pixels, 4 for RGBA
	} else {
		return 0;
	}
}
