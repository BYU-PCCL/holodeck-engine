// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckPawnController.h"
#include "HolodeckSensor.h"
#include "OrientationSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UOrientationSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOrientationSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	

protected:
	virtual void SetDataType() override;

	// Called every frame
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Cache important info
	UPrimitiveComponent* Parent;
	UStaticMeshComponent* RootMesh;
	UWorld* World;

};
