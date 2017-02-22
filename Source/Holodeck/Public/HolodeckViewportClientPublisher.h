// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckViewportClient.h"
#include "HolodeckPawnController.h"
#include "Components/SceneComponent.h"
#include "HolodeckViewportClientPublisher.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UHolodeckViewportClientPublisher : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHolodeckViewportClientPublisher();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	AHolodeckPawnController* Controller;

	UHolodeckViewportClient* ViewportClient;

	TQueue<FString>* ImageQueue;

	UPROPERTY(EditAnywhere)
	bool bGrayScale;
	
};
