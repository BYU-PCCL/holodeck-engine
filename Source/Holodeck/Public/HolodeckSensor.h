// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckPawnController.h"
#include "Components/SceneComponent.h"
#include "HolodeckSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UHolodeckSensor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHolodeckSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
