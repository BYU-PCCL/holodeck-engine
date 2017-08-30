// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckAgent.h"
#include "PressureSensor.h"

#include "Android.generated.h"

UCLASS()
class HOLODECK_API AAndroid : public AHolodeckAgent
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AAndroid();

	/**
	  * Called when the game starts.
	  */
	virtual void BeginPlay() override;
	
	/**
	  * Tick
	  * Called every frame.
	  * @param DeltaSeconds the time since the last tick.
	  */
	void Tick(float DeltaSeconds) override;

	/**
	  * TODO(joshgreaves) : Explain this function properly.
	  * NotifyHit
	  * Used for the pressure sensor.
	  */
	void NotifyHit(UPrimitiveComponent* MyComp,
				   AActor* Other,
				   UPrimitiveComponent* OtherComp,
				   bool bSelfMoved,
				   FVector HitLocation,
				   FVector HitNormal,
				   FVector NormalImpulse,
				   const FHitResult& Hit) override;

	//Decal material. This is used to show collisions on the Android. It is to be left blank and is set programmatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	UMaterial* CollisionDecalMaterial;

	/**
	  * SetCollisionsVisible
	  * Sets whether collisions with the Android can be seen.
	  * @param Visible true to be seen.
	  */
	void SetCollisionsVisible(bool Visible);

	/**
	  * GetCollisionsVisible
	  * @return true if collisions are visible.
	  */
	bool GetCollisionsVisible();

private:
	bool bCollisionsAreVisible;
};
