// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClientPublisher.h"

// Sets default values for this component's properties
UHolodeckViewportClientPublisher::UHolodeckViewportClientPublisher(){
	PrimaryComponentTick.bCanEverTick = true;
}

void UHolodeckViewportClientPublisher::BeginPlay() {
	// This must come first, since the HolodeckSensor parent class will
	// call GetNumItems, which needs the ViewportClient.
	ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient && bOn) {
		const FVector2D ViewportSize = FVector2D(ViewportClient->Viewport->GetSizeXY());
		Width = ViewportSize.X;
		Height = ViewportSize.Y;
		ViewportClient->SetBuffer(Buffer);
	}
	Super::BeginPlay();
}

void UHolodeckViewportClientPublisher::TickSensorComponent(float DeltaTime,
														   ELevelTick TickType,
														   FActorComponentTickFunction* ThisTickFunction) {
	// The pixel data is captured on the rendering thread.
	// All this class needs to do is set the buffer at begin play.
	if (!ViewportClient->BufferIsSet())
		ViewportClient->SetBuffer(Buffer);
}
