// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "HolodeckGameInstance.h"
#include "GameFramework/GameMode.h"
#include "HolodeckGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AHolodeckGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void StartPlay() override;

private:
	UHolodeckGameInstance* Instance;
	
};
