// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherMaster.generated.h"

UCLASS()
class HOLODECK_API AWeatherMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherMaster();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeFogDensity(const float& density);
	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeSunHeight(const float& angle);
	UFUNCTION(BlueprintImplementableEvent)
		bool MakeRain();
	UFUNCTION(BlueprintImplementableEvent)
		bool MakeCloudy();
	/*UPROPERTY(BlueprintReadWrite)
		AWeatherMaster* WeatherMaster;*/


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
