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
	if (ViewportClient) {
		// ImageQueue = &(ViewportClient->ImageQueue);
		ViewportClient->bGrayScale = this->bGrayScale;
	}
}


// Called every frame
void UHolodeckViewportClientPublisher::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	UE_LOG(LogTemp, Warning, TEXT("buffer size is %d"), GetDataLength());
	if (ViewportClient) {
		for (int i = 0; i < ViewportClient->HolodeckColorBuffer.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("position: %d - %d"), 3 * i, 3 * i + 2);
			buffer[3 * i] = (float)ViewportClient->HolodeckColorBuffer[i].R / 255.0f;
			buffer[3 * i + 1] = (float)ViewportClient->HolodeckColorBuffer[i].G / 255.0f;
			buffer[3 * i + 2] = (float)ViewportClient->HolodeckColorBuffer[i].B / 255.0f;
		}
	}
	/*
	if (ViewportClient) {
		while (!ImageQueue->IsEmpty())
			ImageQueue->Dequeue(ResultData.Data);
		ResultData.Data = FString("\"") + ResultData.Data + FString("\"");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find HolodeckViewportClient"));
	}*/
}

int UHolodeckViewportClientPublisher::GetDataLength() {
	return 256 * 256 * 4 * 3;
	if (ViewportClient)
	{
		FVector2D Dims;
		ViewportClient->GetViewportSize(Dims);
		return Dims.X * Dims.Y * sizeof(float) * 3; // X by Y pixels, 3 for RGB
	}
	else {
		return 0;
	}
}

