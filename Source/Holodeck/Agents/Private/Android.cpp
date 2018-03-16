// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"

// Used to convert unreal torque unit (kg*cm^2/s^2) to (kg*m^2/s^2)
const float CM_TORQUE_TO_M_TORQUE = 10000;

const int NUM_JOINTS = 48;

AAndroid::AAndroid() {
	PrimaryActorTick.bCanEverTick = true;
	bCollisionsAreVisible = false;
}

void AAndroid::BeginPlay() {
	Super::BeginPlay();
	count = 0;
	SkeletalMesh = Cast<USkeletalMeshComponent>(RootComponent);
	BoneNames = SkeletalMesh->GetAllSocketNames();
	Constraints = SkeletalMesh->Constraints;
}

// Called every frame
void AAndroid::Tick(float DeltaTime) {
	Super::Tick( DeltaTime );
	ApplyTorques();
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

void AAndroid::ApplyTorques() {

	for (int i = 0; i < Constraints.Num(); i++) {

		int BegComInd = i * 3;
		FName JointName = Constraints[i]->JointName;

		// Get rotation of that socket
		FRotator JointRotation = SkeletalMesh->GetSocketRotation(JointName);
		FConstraintInstance* Constraint = Constraints[i];
		FTransform JointTransform = Constraint->GetRefFrame(EConstraintFrame::Frame1);

		// Get the proper Vector based on the axis
		FVector Swing1TorqVec = JointTransform.GetScaledAxis(EAxis::X);
		FVector Swing2TorqVec = JointTransform.GetScaledAxis(EAxis::Y);
		FVector TwistTorqVec = JointTransform.GetScaledAxis(EAxis::Z);

		// Convert Torque magnitude to (Kg*m^2)/(s^2) and scale with commands
		Swing1TorqVec *= (CM_TORQUE_TO_M_TORQUE * CommandArray[BegComInd]);
		Swing2TorqVec *= (CM_TORQUE_TO_M_TORQUE * CommandArray[BegComInd + 1]);
		TwistTorqVec *= (CM_TORQUE_TO_M_TORQUE * CommandArray[BegComInd + 2]);

		// Add Torques to the mesh
		SkeletalMesh->AddTorque(Swing1TorqVec, JointName, false);
		SkeletalMesh->AddTorque(Swing2TorqVec, JointName, false);
		SkeletalMesh->AddTorque(TwistTorqVec, JointName, false);
	}
}
