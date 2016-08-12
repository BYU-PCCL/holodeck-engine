// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PressureSensor.h"
#include "Android.generated.h"

UCLASS()
class HOLODECK_API AAndroid : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAndroid();

	USkeletalMeshComponent* SkeletalMeshComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void NotifyHit(
		UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
		) override;

	UPressureSensor* PressureSensor;

	//Decal material. This is used to show collisions on the Android. It is to be left blank and is set programmatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterial* CollisionDecalMaterial;

	void SetCollisionsVisible(bool value);

	bool GetCollisionsVisible();

private:
	bool AreCollisionsVisible;
};
