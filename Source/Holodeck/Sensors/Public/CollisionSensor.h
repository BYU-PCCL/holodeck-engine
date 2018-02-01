// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Holodeck.h"

#include <string>

#include "Components/SceneComponent.h"
#include "HolodeckSensor.h"
#include "CollisionSensor.generated.h"

/**
* VelocitySensor
* Inherits from the HolodeckSensor class
* Check out the parent class for documentation on all of the overridden funcions.
* Gets the true velocity of the component that the sensor is attached to.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UCollisionSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	UCollisionSensor();

	/**
	* BeginPlay
	* Called at the start of the game.
	*/
	void BeginPlay() override;


	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	//See HolodeckSensor for the documentation of these overridden functions.
	FString GetDataKey() override { return "CollisionSensor"; };
	int GetNumItems() override { return 1; };
	int GetItemSize() override { return sizeof(bool); };
	void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/**
	* Parent
	* After initialization, Parent contains a pointer to whatever the sensor is attached to.
	* Not owned.
	*/
	AActor* Parent;

	bool bIsColliding = false;

};
