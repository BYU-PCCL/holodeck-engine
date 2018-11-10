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
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;
	
protected:
	// See HolodeckSensor for documentation for these overridden functions.
	FString GetDataKey() override { return "PrimaryPlayerCamera"; };
	int GetNumItems() override { return Width * Height; };
	int GetItemSize() override { return sizeof(FColor); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool bGrayScale;

	UPROPERTY(EditAnywhere)
	int Width;

	UPROPERTY(EditAnywhere)
	int Height;

private:
	UHolodeckViewportClient* ViewportClient;
};
