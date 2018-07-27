// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "WeatherMaster.h"


// Sets default values
AWeatherMaster::AWeatherMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeatherMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeatherMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

