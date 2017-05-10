// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckTask.h"


// Sets default values
AHolodeckTask::AHolodeckTask()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainAgent = nullptr;

}

// Called when the game starts or when spawned
void AHolodeckTask::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHolodeckTask::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

