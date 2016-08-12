// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckSensor.h"
#include "HolodeckIMUSensor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UHolodeckIMUSensor : public UHolodeckSensor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UHolodeckIMUSensor();
	/*
	AHolodeckPawnController* Controller;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CalculateAccelerationVector(float DeltaTime);

	void CalculateAngularVelocityVector();

	FVector GetAccelerationVector();

	FVector GetAngularVelocityVector();

	UPrimitiveComponent* Parent;

	UWorld* World;
	AWorldSettings* WorldSettings;
	float WorldToMetersRatio;
	float WorldGravity;

	FVector VelocityThen;
	FVector VelocityNow;

	FRotator RotationNow;

	FVector LinearAccelerationVector;

	FVector AngularVelocityVector;

	void PublishSensorMessage();
	*/
};
