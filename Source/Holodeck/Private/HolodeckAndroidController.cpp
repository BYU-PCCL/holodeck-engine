// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"
#include "HolodeckAndroidController.h"
#include "HolodeckWorldSettings.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {

}

void AHolodeckAndroidController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);

	TArray<USkeletalMeshComponent*> components;
	Pawn->GetComponents<USkeletalMeshComponent>(components);
	SkeletalMeshComponent = components[0];
}


void AHolodeckAndroidController::SetJointConstraint(FName joint, float swing1, float twist, float swing2, float force) {

	FConstraintInstance* Constraint = SkeletalMeshComponent->FindConstraintInstance(joint);
	Constraint->SetAngularPositionDrive(true, true);
	Constraint->SetAngularDriveParams(100000 * force, 10000, 100000);

	Constraint->SetAngularOrientationTarget(
						FRotator(Constraint->Swing2LimitAngle * swing2, // Blue fan in Unreal Editor
								 Constraint->Swing1LimitAngle * swing1, // Red fan
								 Constraint->TwistLimitAngle * twist)  // Green fan
								.Quaternion());
}


void AHolodeckAndroidController::OnReceiveCommand(const FHolodeckAndroidCommand& Command) {
	
	TArray<float> commandArray = Command.ConstraintVector;

	if(commandArray.Num() > 1){
		SetJointConstraint(FName(TEXT("head"       )), commandArray[  0], commandArray[  1], commandArray[  2], commandArray[  3]);
		SetJointConstraint(FName(TEXT("neck_01"    )), commandArray[  4],				  0,			     0, commandArray[  5]);
		SetJointConstraint(FName(TEXT("spine_02"   )), commandArray[  6], commandArray[  7],				 0, commandArray[  8]);
		SetJointConstraint(FName(TEXT("spine_01"   )), commandArray[  9], commandArray[ 10], commandArray[ 11], commandArray[ 12]);
		SetJointConstraint(FName(TEXT("upperarm_l" )), commandArray[ 13], commandArray[ 14], commandArray[ 15], commandArray[ 16]);
		SetJointConstraint(FName(TEXT("lowerarm_l" )), commandArray[ 17],				  0,				 0, commandArray[ 18]);
		SetJointConstraint(FName(TEXT("hand_l"     )), commandArray[ 19], commandArray[ 20], commandArray[ 21], commandArray[ 22]);
		SetJointConstraint(FName(TEXT("thumb_01_l" )), commandArray[ 23],				  0, commandArray[ 24], commandArray[ 25]);
		SetJointConstraint(FName(TEXT("thumb_02_l" )), commandArray[ 26],                 0,                 0, commandArray[ 27]);
		SetJointConstraint(FName(TEXT("thumb_03_l" )), commandArray[ 28],                 0,                 0, commandArray[ 29]);
		SetJointConstraint(FName(TEXT("index_01_l" )), commandArray[ 30],                 0, commandArray[ 31], commandArray[ 32]);
		SetJointConstraint(FName(TEXT("index_02_l" )), commandArray[ 33],                 0,                 0, commandArray[ 34]);
		SetJointConstraint(FName(TEXT("index_03_l" )), commandArray[ 35],                 0,                 0, commandArray[ 36]);
		SetJointConstraint(FName(TEXT("middle_01_l")), commandArray[ 37],                 0, commandArray[ 38], commandArray[ 39]);
		SetJointConstraint(FName(TEXT("middle_02_l")), commandArray[ 40],                 0,                 0, commandArray[ 41]);
		SetJointConstraint(FName(TEXT("middle_03_l")), commandArray[ 42],                 0,                 0, commandArray[ 43]);
		SetJointConstraint(FName(TEXT("ring_01_l"  )), commandArray[ 44],                 0, commandArray[ 45], commandArray[ 46]);
		SetJointConstraint(FName(TEXT("ring_02_l"  )), commandArray[ 47],                 0,                 0, commandArray[ 48]);
		SetJointConstraint(FName(TEXT("ring_03_l"  )), commandArray[ 49],                 0,                 0, commandArray[ 50]);
		SetJointConstraint(FName(TEXT("pinky_01_l" )), commandArray[ 51],                 0, commandArray[ 52], commandArray[ 53]);
		SetJointConstraint(FName(TEXT("pinky_02_l" )), commandArray[ 54],                 0,                 0, commandArray[ 55]);
		SetJointConstraint(FName(TEXT("pinky_03_l" )), commandArray[ 56],                 0,                 0, commandArray[ 57]);
		SetJointConstraint(FName(TEXT("upperarm_r" )), commandArray[ 58], commandArray[ 59], commandArray[ 60], commandArray[ 61]);
		SetJointConstraint(FName(TEXT("lowerarm_r" )), commandArray[ 62],                 0,                 0, commandArray[ 63]);
		SetJointConstraint(FName(TEXT("hand_r"     )), commandArray[ 64], commandArray[ 65], commandArray[ 66], commandArray[ 67]);
		SetJointConstraint(FName(TEXT("thumb_01_r" )), commandArray[ 68],                 0, commandArray[ 69], commandArray[ 70]);
		SetJointConstraint(FName(TEXT("thumb_02_r" )), commandArray[ 71],                 0,                 0, commandArray[ 72]);
		SetJointConstraint(FName(TEXT("thumb_03_r" )), commandArray[ 73],                 0,                 0, commandArray[ 74]);
		SetJointConstraint(FName(TEXT("index_01_r" )), commandArray[ 75],                 0, commandArray[ 76], commandArray[ 77]);
		SetJointConstraint(FName(TEXT("index_02_r" )), commandArray[ 78],                 0,                 0, commandArray[ 79]);
		SetJointConstraint(FName(TEXT("index_03_r" )), commandArray[ 80],                 0,                 0, commandArray[ 81]);
		SetJointConstraint(FName(TEXT("middle_01_r")), commandArray[ 82],                 0, commandArray[ 83], commandArray[ 84]);
		SetJointConstraint(FName(TEXT("middle_02_r")), commandArray[ 85],                 0,                 0, commandArray[ 86]);
		SetJointConstraint(FName(TEXT("middle_03_r")), commandArray[ 87],                 0,                 0, commandArray[ 88]);
		SetJointConstraint(FName(TEXT("ring_01_r"  )), commandArray[ 89],                 0, commandArray[ 90], commandArray[ 91]);
		SetJointConstraint(FName(TEXT("ring_02_r"  )), commandArray[ 92],                 0,                 0, commandArray[ 93]);
		SetJointConstraint(FName(TEXT("ring_03_r"  )), commandArray[ 94],                 0,                 0, commandArray[ 95]);
		SetJointConstraint(FName(TEXT("pinky_01_r" )), commandArray[ 96],                 0, commandArray[ 97], commandArray[ 98]);
		SetJointConstraint(FName(TEXT("pinky_02_r" )), commandArray[ 99],                 0,                 0, commandArray[100]);
		SetJointConstraint(FName(TEXT("pinky_03_r" )), commandArray[101],                 0,                 0, commandArray[102]);
		SetJointConstraint(FName(TEXT("thigh_l"    )), commandArray[103], commandArray[104], commandArray[105], commandArray[106]);
		SetJointConstraint(FName(TEXT("calf_l"     )), commandArray[107],                 0,                 0, commandArray[108]);
		SetJointConstraint(FName(TEXT("foot_l"     )), commandArray[109],                 0, commandArray[110], commandArray[111]);
		SetJointConstraint(FName(TEXT("ball_l"     )), commandArray[112], commandArray[113],                 0, commandArray[114]);
		SetJointConstraint(FName(TEXT("thigh_r"    )), commandArray[115], commandArray[116], commandArray[117], commandArray[118]);
		SetJointConstraint(FName(TEXT("calf_r"     )), commandArray[119],                 0,                 0, commandArray[120]);
		SetJointConstraint(FName(TEXT("foot_r"     )), commandArray[121],                 0, commandArray[122], commandArray[123]);
		SetJointConstraint(FName(TEXT("ball_r"     )), commandArray[124], commandArray[125],                 0, commandArray[126]);
	}
}

void AHolodeckAndroidController::OnReceiveConfiguration(const FHolodeckAndroidConfiguration& Configuration) {

	((AAndroid*) this->GetPawn())->SetCollisionsVisible(Configuration.AreCollisionsVisible);

}


