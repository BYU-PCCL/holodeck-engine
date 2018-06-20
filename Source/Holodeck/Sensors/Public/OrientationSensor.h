// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "HolodeckPawnController.h"
#include "HolodeckSensor.h"

#include "OrientationSensor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

/**
* UOrientationSensor
* Inherits from the HolodeckSensor class
* Check out the parent class for documentation on all of the overridden funcions.
* Gives the complete orientation of the parent agent in three vectors: forward, right, and up.
*/
class HOLODECK_API UOrientationSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:	
	/**
	  * Default Constructor.
	  */
	UOrientationSensor();

	/**
	  * Called at the start of the game.
	  */
	void BeginPlay() override;
	
protected:
	// See HolodeckSensor for documentation on these classes.
	FString GetDataKey() override { return "OrientationSensor"; };
	int GetNumItems() override { return 9; };
	int GetItemSize() override { return sizeof(float); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPrimitiveComponent* Parent;
	UStaticMeshComponent* RootMesh;
	UWorld* World;
};
