// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HolodeckViewportClient.h"
#include "Components/SceneComponent.h"
#include "HolodeckPawnController.h"
#include "CameraSensorArray2D.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UCameraSensorArray2D : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraSensorArray2D();

	AHolodeckPawnController* Controller;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	bool Capture(TMap<FString, FString>& output);

private:
	TArray<USceneCaptureComponent2D*> AAttachedSceneCaptureComponents2D;
	TArray<UCameraComponent*> AAttachedCameras;

};

