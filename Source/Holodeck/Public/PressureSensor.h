// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "HolodeckPawnController.h"
#include "PressureSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UPressureSensor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPressureSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void HandleNotifyHit(USkeletalMeshComponent*& SkeletalMeshComponent, FVector NormalImpulse, const FHitResult& Hit);

private:

	AHolodeckPawnController* Controller;

	TMap<FString, TArray<FString>> HitsMap;
	
};
