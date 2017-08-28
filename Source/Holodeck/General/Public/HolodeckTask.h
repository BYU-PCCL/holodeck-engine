// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HolodeckAgent.h"
#include "HolodeckTask.generated.h"

UCLASS()
class HOLODECK_API AHolodeckTask : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHolodeckTask();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float reward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool terminal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AHolodeckAgent* MainAgent;
};
