// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckViewportClient.h"
#include "HolodeckSensor.h"

#include "HolodeckViewportClientPublisher.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UHolodeckViewportClientPublisher : public UHolodeckSensor {
	GENERATED_BODY()

public:	
	/**
	  * Default Constructor.
	  */
	UHolodeckViewportClientPublisher();

	/**
	  * BeginPlay
	  * Called at the start of the game.
	  */
	void BeginPlay() override;
	
protected:
	// See HolodeckSensor for documentation for these overridden functions.
	FString GetDataKey() override { return "PrimaryPlayerCamera"; };
	int GetNumItems() override;
	int GetItemSize() override { return sizeof(FColor); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		bool bGrayScale;

private:
	UHolodeckViewportClient* ViewportClient;
};
