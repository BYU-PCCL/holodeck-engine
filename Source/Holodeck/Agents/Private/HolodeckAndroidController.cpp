// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAndroidController.h"

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {}

void AHolodeckAndroidController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);

	TArray<USkeletalMeshComponent*> Components;
	PawnParam->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMeshComponent = Components[0];
}

void AHolodeckAndroidController::SetJointConstraint(FName Joint, float Swing1, float Twist, float Swing2, float Force) {
	// TODO(joshgreaves) : Fix the magic numbers here, when I understand them.
	FConstraintInstance* Constraint = SkeletalMeshComponent->FindConstraintInstance(Joint);
	Constraint->SetOrientationDriveTwistAndSwing(true, true);
	Constraint->SetAngularDriveParams(100000 * Force, 10000, 100000);

	Constraint->SetAngularOrientationTarget(
						FRotator(Constraint->GetAngularSwing2Limit() * Swing2, // Blue fan in Unreal Editor
								 Constraint->GetAngularSwing1Limit() * Swing1, // Red fan
								 Constraint->GetAngularTwistLimit() * Twist)  // Green fan
								.Quaternion());
}

void AHolodeckAndroidController::ExecuteCommand() {
	if (ActionBuffer != nullptr) {
		float* CommandArray = static_cast<float*>(ActionBuffer);

		SetJointConstraint(FName(TEXT("head"       )), CommandArray[  0], CommandArray[  1], CommandArray[  2], CommandArray[  3]);
		SetJointConstraint(FName(TEXT("neck_01"    )), CommandArray[  4],				  0,			     0, CommandArray[  5]);
		SetJointConstraint(FName(TEXT("spine_02"   )), CommandArray[  6], CommandArray[  7],				 0, CommandArray[  8]);
		SetJointConstraint(FName(TEXT("spine_01"   )), CommandArray[  9], CommandArray[ 10], CommandArray[ 11], CommandArray[ 12]);
		SetJointConstraint(FName(TEXT("upperarm_l" )), CommandArray[ 13], CommandArray[ 14], CommandArray[ 15], CommandArray[ 16]);
		SetJointConstraint(FName(TEXT("lowerarm_l" )), CommandArray[ 17],				  0,				 0, CommandArray[ 18]);
		SetJointConstraint(FName(TEXT("hand_l"     )), CommandArray[ 19], CommandArray[ 20], CommandArray[ 21], CommandArray[ 22]);
		SetJointConstraint(FName(TEXT("thumb_01_l" )), CommandArray[ 23],				  0, CommandArray[ 24], CommandArray[ 25]);
		SetJointConstraint(FName(TEXT("thumb_02_l" )), CommandArray[ 26],                 0,                 0, CommandArray[ 27]);
		SetJointConstraint(FName(TEXT("thumb_03_l" )), CommandArray[ 28],                 0,                 0, CommandArray[ 29]);
		SetJointConstraint(FName(TEXT("index_01_l" )), CommandArray[ 30],                 0, CommandArray[ 31], CommandArray[ 32]);
		SetJointConstraint(FName(TEXT("index_02_l" )), CommandArray[ 33],                 0,                 0, CommandArray[ 34]);
		SetJointConstraint(FName(TEXT("index_03_l" )), CommandArray[ 35],                 0,                 0, CommandArray[ 36]);
		SetJointConstraint(FName(TEXT("middle_01_l")), CommandArray[ 37],                 0, CommandArray[ 38], CommandArray[ 39]);
		SetJointConstraint(FName(TEXT("middle_02_l")), CommandArray[ 40],                 0,                 0, CommandArray[ 41]);
		SetJointConstraint(FName(TEXT("middle_03_l")), CommandArray[ 42],                 0,                 0, CommandArray[ 43]);
		SetJointConstraint(FName(TEXT("ring_01_l"  )), CommandArray[ 44],                 0, CommandArray[ 45], CommandArray[ 46]);
		SetJointConstraint(FName(TEXT("ring_02_l"  )), CommandArray[ 47],                 0,                 0, CommandArray[ 48]);
		SetJointConstraint(FName(TEXT("ring_03_l"  )), CommandArray[ 49],                 0,                 0, CommandArray[ 50]);
		SetJointConstraint(FName(TEXT("pinky_01_l" )), CommandArray[ 51],                 0, CommandArray[ 52], CommandArray[ 53]);
		SetJointConstraint(FName(TEXT("pinky_02_l" )), CommandArray[ 54],                 0,                 0, CommandArray[ 55]);
		SetJointConstraint(FName(TEXT("pinky_03_l" )), CommandArray[ 56],                 0,                 0, CommandArray[ 57]);
		SetJointConstraint(FName(TEXT("upperarm_r" )), CommandArray[ 58], CommandArray[ 59], CommandArray[ 60], CommandArray[ 61]);
		SetJointConstraint(FName(TEXT("lowerarm_r" )), CommandArray[ 62],                 0,                 0, CommandArray[ 63]);
		SetJointConstraint(FName(TEXT("hand_r"     )), CommandArray[ 64], CommandArray[ 65], CommandArray[ 66], CommandArray[ 67]);
		SetJointConstraint(FName(TEXT("thumb_01_r" )), CommandArray[ 68],                 0, CommandArray[ 69], CommandArray[ 70]);
		SetJointConstraint(FName(TEXT("thumb_02_r" )), CommandArray[ 71],                 0,                 0, CommandArray[ 72]);
		SetJointConstraint(FName(TEXT("thumb_03_r" )), CommandArray[ 73],                 0,                 0, CommandArray[ 74]);
		SetJointConstraint(FName(TEXT("index_01_r" )), CommandArray[ 75],                 0, CommandArray[ 76], CommandArray[ 77]);
		SetJointConstraint(FName(TEXT("index_02_r" )), CommandArray[ 78],                 0,                 0, CommandArray[ 79]);
		SetJointConstraint(FName(TEXT("index_03_r" )), CommandArray[ 80],                 0,                 0, CommandArray[ 81]);
		SetJointConstraint(FName(TEXT("middle_01_r")), CommandArray[ 82],                 0, CommandArray[ 83], CommandArray[ 84]);
		SetJointConstraint(FName(TEXT("middle_02_r")), CommandArray[ 85],                 0,                 0, CommandArray[ 86]);
		SetJointConstraint(FName(TEXT("middle_03_r")), CommandArray[ 87],                 0,                 0, CommandArray[ 88]);
		SetJointConstraint(FName(TEXT("ring_01_r"  )), CommandArray[ 89],                 0, CommandArray[ 90], CommandArray[ 91]);
		SetJointConstraint(FName(TEXT("ring_02_r"  )), CommandArray[ 92],                 0,                 0, CommandArray[ 93]);
		SetJointConstraint(FName(TEXT("ring_03_r"  )), CommandArray[ 94],                 0,                 0, CommandArray[ 95]);
		SetJointConstraint(FName(TEXT("pinky_01_r" )), CommandArray[ 96],                 0, CommandArray[ 97], CommandArray[ 98]);
		SetJointConstraint(FName(TEXT("pinky_02_r" )), CommandArray[ 99],                 0,                 0, CommandArray[100]);
		SetJointConstraint(FName(TEXT("pinky_03_r" )), CommandArray[101],                 0,                 0, CommandArray[102]);
		SetJointConstraint(FName(TEXT("thigh_l"    )), CommandArray[103], CommandArray[104], CommandArray[105], CommandArray[106]);
		SetJointConstraint(FName(TEXT("calf_l"     )), CommandArray[107],                 0,                 0, CommandArray[108]);
		SetJointConstraint(FName(TEXT("foot_l"     )), CommandArray[109],                 0, CommandArray[110], CommandArray[111]);
		SetJointConstraint(FName(TEXT("ball_l"     )), CommandArray[112], CommandArray[113],                 0, CommandArray[114]);
		SetJointConstraint(FName(TEXT("thigh_r"    )), CommandArray[115], CommandArray[116], CommandArray[117], CommandArray[118]);
		SetJointConstraint(FName(TEXT("calf_r"     )), CommandArray[119],                 0,                 0, CommandArray[120]);
		SetJointConstraint(FName(TEXT("foot_r"     )), CommandArray[121],                 0, CommandArray[122], CommandArray[123]);
		SetJointConstraint(FName(TEXT("ball_r"     )), CommandArray[124], CommandArray[125],                 0, CommandArray[126]);
	}
}
