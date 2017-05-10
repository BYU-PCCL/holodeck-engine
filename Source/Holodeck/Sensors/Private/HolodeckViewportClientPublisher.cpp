// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClientPublisher.h"


// Sets default values for this component's properties
UHolodeckViewportClientPublisher::UHolodeckViewportClientPublisher()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHolodeckViewportClientPublisher::BeginPlay()
{
	Super::BeginPlay();

	ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient) {
		ImageQueue = &(ViewportClient->ImageQueue);
		ViewportClient->bGrayScale = this->bGrayScale;
	}
}


// Called every frame
void UHolodeckViewportClientPublisher::TickSensorComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	if (ViewportClient) {
		while (!ImageQueue->IsEmpty())
			ImageQueue->Dequeue(ResultData.Data);
		ResultData.Data = FString("\"") + ResultData.Data + FString("\"");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find HolodeckViewportClient"));
	}
}

int UHolodeckViewportClientPublisher::GetDataLength() {
	if (ViewportClient)
	{
		FVector2D Dims;
		ViewportClient->GetViewportSize(Dims);
		return Dims.X * Dims.Y * 4 * 2; // X by Y pixels, 4 for RGBA, 2 for uint8
	}
	else {
		return 0;
	}
}

