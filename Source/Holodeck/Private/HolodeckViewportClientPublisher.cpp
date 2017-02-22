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

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);
	ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient) {
		ImageQueue = &(ViewportClient->ImageQueue);
		ViewportClient->bGrayScale = this->bGrayScale;
	}
}


// Called every frame
void UHolodeckViewportClientPublisher::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (ViewportClient) {
		//TArray<FColor> colorArray = TArray<FColor>(ViewportClient->HolodeckColorBuffer);

		FHolodeckSensorData data = FHolodeckSensorData();
		data.Type = "PrimaryPlayerCamera";
		while (!ImageQueue->IsEmpty())
			ImageQueue->Dequeue(data.Data);
		data.Data = FString("\"") + data.Data + FString("\"");
		Controller->Publish(data);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "camera broken");
	}
}

