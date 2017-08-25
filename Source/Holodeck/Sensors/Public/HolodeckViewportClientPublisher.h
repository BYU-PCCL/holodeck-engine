// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckViewportClient.h"
#include "HolodeckSensor.h"
#include "HolodeckViewportClientPublisher.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UHolodeckViewportClientPublisher : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHolodeckViewportClientPublisher();

	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	virtual FString GetDataKey() override { return "PrimaryPlayerCamera"; };
	virtual int GetDataLength() override;

	// Called every frame
	virtual void TickSensorComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere)
		bool bGrayScale;

private:
	UHolodeckViewportClient* ViewportClient;

	// TQueue<FString>* ImageQueue;
};
