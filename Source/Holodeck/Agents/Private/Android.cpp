// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"

// Used to convert unreal torque unit (kg*cm^2/s^2) to (kg*m^2/s^2)
const float CM_TORQUE_TO_M_TORQUE = 10000;

AAndroid::AAndroid() {
	PrimaryActorTick.bCanEverTick = true;
	bCollisionsAreVisible = false;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.AndroidController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
}

void AAndroid::InitializeAgent() {
	Super::InitializeAgent();
	SkeletalMesh = Cast<USkeletalMeshComponent>(RootComponent);
}

void AAndroid::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	ApplyTorques();
}

void AAndroid::SetCollisionsVisible(bool Visible) {
	bCollisionsAreVisible = Visible;
}

bool AAndroid::GetCollisionsVisible() {
	return bCollisionsAreVisible;
}

void AAndroid::ApplyTorques() {
	UE_LOG(LogHolodeck, Verbose, TEXT("AAndroid::ApplyTorques"));
	int ComInd = 0;

	for (int JointInd = 0; JointInd < NUM_JOINTS; JointInd++) {

		FName JointName = Joints[JointInd];

		// Get rotation of that socket
		FQuat RotQuat = SkeletalMesh->GetSocketQuaternion(JointName);

		// Apply Swing 1 Torque if non zero
		if (CommandArray[ComInd] != 0) {
			float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
			SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, 0.0f, RotForce)), JointName, false);
		}
		ComInd++;

		// Apply Swing 2 if Torque non zero and is 2 or 3 axis joint
		if (JointInd < (NUM_2_PLUS_3_AXIS_JOINTS)) {
			if (CommandArray[ComInd] != 0) {
				float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
				SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, RotForce, 0.0f)), JointName, false);
			}
			ComInd++;

			// Apply Twist if Torque non zero and is 3 axis joint
			if (JointInd < NUM_3_AXIS_JOINTS) {
				if (CommandArray[ComInd] != 0) {
					float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
					SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(RotForce, 0.0f, 0.0f)), JointName, false);
				}
				ComInd++;
			}
		}
	}
}

const FName AAndroid::Joints[] = {

	// Head, Spine, and Arm joints. Each has [swing1, swing2, twist]
	FName(TEXT("head")),
	FName(TEXT("neck_01")),
	FName(TEXT("spine_02")),
	FName(TEXT("spine_01")),
	FName(TEXT("upperarm_l")),
	FName(TEXT("lowerarm_l")),
	FName(TEXT("hand_l")),
	FName(TEXT("upperarm_r")),
	FName(TEXT("lowerarm_r")),
	FName(TEXT("hand_r")),

	// Leg Joints. Each has [swing1, swing2, twist]
	FName(TEXT("thigh_l")),
	FName(TEXT("calf_l")),
	FName(TEXT("foot_l")),
	FName(TEXT("ball_l")),
	FName(TEXT("thigh_r")),
	FName(TEXT("calf_r")),
	FName(TEXT("foot_r")),
	FName(TEXT("ball_r")),

	// First joint of each finger. Has only [swing1, swing2]
	FName(TEXT("thumb_01_l")),
	FName(TEXT("index_01_l")),
	FName(TEXT("middle_01_l")),
	FName(TEXT("ring_01_l")),
	FName(TEXT("pinky_01_l")),
	FName(TEXT("thumb_01_r")),
	FName(TEXT("index_01_r")),
	FName(TEXT("middle_01_r")),
	FName(TEXT("ring_01_r")),
	FName(TEXT("pinky_01_r")),

	// Second joint of each finger. Has only [swing1]
	FName(TEXT("thumb_02_l")),
	FName(TEXT("index_02_l")),
	FName(TEXT("middle_02_l")),
	FName(TEXT("ring_02_l")),
	FName(TEXT("pinky_02_l")),
	FName(TEXT("thumb_02_r")),
	FName(TEXT("index_02_r")),
	FName(TEXT("middle_02_r")),
	FName(TEXT("ring_02_r")),
	FName(TEXT("pinky_02_r")),

	// Third joint of each finger. Has only [swing1]
	FName(TEXT("thumb_03_l")),
	FName(TEXT("index_03_l")),
	FName(TEXT("middle_03_l")),
	FName(TEXT("ring_03_l")),
	FName(TEXT("pinky_03_l")),
	FName(TEXT("thumb_03_r")),
	FName(TEXT("index_03_r")),
	FName(TEXT("middle_03_r")),
	FName(TEXT("ring_03_r")),
	FName(TEXT("pinky_03_r")),
};
