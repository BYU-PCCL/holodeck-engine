// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckSensor.h"
#include "Android.h"

#include "Components/SceneComponent.h"

#include "PressureSensor.generated.h"

// 94 DOF each with 2 length 3 vectors containing impulse normal info and hit location
static const int NUM_JOINTS = 48;
const static int NUM_PRESSURE_ITEMS = NUM_JOINTS * (3 + 1);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/**
* UPressureSensor
*/
class HOLODECK_API UPressureSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressureSensor();

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;
	
	/**
	 * Callback function that is called whenever part of the actor is hit. 
	 */
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:

	float PrivateData[NUM_PRESSURE_ITEMS];

protected:
	// 94 DOF each with 2 length 3 vectors containing impulse normal info and hit location
	int GetNumItems() override { return NUM_PRESSURE_ITEMS; };
	int GetItemSize() override { return sizeof(float); };

	// Called every frame
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float* AddHitToBuffer(FString BoneName, FVector HitBoneLocation, FVector NormalImpulse, float* Data);

	AAndroid* Android;

	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitJointMap();
	TMap<FString, int> JointMap;
};
