// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"

AAndroid::AAndroid() {
	PrimaryActorTick.bCanEverTick = true;
	bCollisionsAreVisible = false;
}

void AAndroid::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AAndroid::Tick(float DeltaTime) {
	Super::Tick( DeltaTime );
}

void AAndroid::NotifyHit(UPrimitiveComponent* MyComp,
						 AActor* Other,
						 UPrimitiveComponent* OtherComp,
						 bool bSelfMoved,
						 FVector HitLocation,
						 FVector HitNormal,
						 FVector NormalImpulse,
						 const FHitResult& Hit) {
	if (bCollisionsAreVisible) {
		UMaterialInstanceDynamic* TheMaterial_Dyn = UMaterialInstanceDynamic::Create(CollisionDecalMaterial, this);
		
		//FRotator rotator = (HitLocation.Rotation());
		if (TheMaterial_Dyn != NULL) //FVector needs to be at least around 7,7,7 for some reason
			UGameplayStatics::SpawnDecalAttached(TheMaterial_Dyn, FVector(7.5, 7.5, 7.5),
												 MyComp, Hit.BoneName, HitLocation, HitLocation.Rotation(),
												 EAttachLocation::KeepWorldPosition, 1.0);
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Material for decal is null");
	}
}

void AAndroid::SetCollisionsVisible(bool Visible){
	bCollisionsAreVisible = Visible;
}

bool AAndroid::GetCollisionsVisible() {
	return bCollisionsAreVisible;
}