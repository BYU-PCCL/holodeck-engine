// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"


// Sets default values
AAndroid::AAndroid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//showCollisions default is false
	AreCollisionsVisible = false;

}

// Called when the game starts or when spawned
void AAndroid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAndroid::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AAndroid::SetupPlayerInputComponent(class UInputComponent* component)
{
	Super::SetupPlayerInputComponent(component);

}

void AAndroid::NotifyHit(
	UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (AreCollisionsVisible) {

		UMaterialInstanceDynamic* TheMaterial_Dyn = UMaterialInstanceDynamic::Create(CollisionDecalMaterial, this);

		//FRotator rotator = (HitLocation.Rotation());

		if (TheMaterial_Dyn != NULL) { //FVector needs to be at least around 7,7,7 for some reason
			UGameplayStatics::SpawnDecalAttached(TheMaterial_Dyn, FVector(7.5, 7.5, 7.5), MyComp, Hit.BoneName, HitLocation, HitLocation.Rotation(), EAttachLocation::KeepWorldPosition, 1.0);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Material for decal is null");
		}

	}

}

void AAndroid::SetCollisionsVisible(bool value)
{
	AreCollisionsVisible = value;
}

bool AAndroid::GetCollisionsVisible()
{
	return AreCollisionsVisible;
}