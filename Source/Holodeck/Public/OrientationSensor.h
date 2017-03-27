// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckPawnController.h"
#include "Components/SceneComponent.h"
#include "OrientationSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UOrientationSensor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrientationSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// The controller for the agent
	AHolodeckPawnController* Controller;

	UPrimitiveComponent* Parent;
	UStaticMeshComponent* RootMesh;
	UWorld* World;

};
