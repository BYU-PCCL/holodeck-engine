// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "HolodeckSensor.h"
#include "PressureSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
/**
* UPressureSensor
* This class is currently broken. 
*/
class HOLODECK_API UPressureSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressureSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	FString GetDataKey() override { return "PressureSensor"; };
	int GetNumItems() override { return 10000; };
	int GetItemSize() override { return sizeof(float); };

	// Called every frame
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TMap<FString, TArray<FString>> HitsMap;

	USkeletalMeshComponent* SkeletalMeshComponent;
	
};
