// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAndroidController.h"

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {}

void AHolodeckAndroidController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);

	TArray<USkeletalMeshComponent*> Components;
	Pawn->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMeshComponent = Components[0];
}

void AHolodeckAndroidController::ExecuteCommand() {

	float* FloatPtr = static_cast<float*>(ActionBuffer);

	AAndroid* AndroidPawn = Cast<AAndroid>(GetPawn());
	if (AndroidPawn != nullptr && FloatPtr != nullptr) {
		AndroidPawn->CommandArray = FloatPtr;
	}

<<<<<<< HEAD
=======
		// Get rotation of that socket
		FQuat RotQuat = SkeletalMesh->GetSocketQuaternion(JointName);

		// Apply Swing 1 Torque if non zero
		if (CommandArray[ComInd] != 0) {
			float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
			SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, 0.0f, RotForce)), JointName, false);
		}
		ComInd++;

		// Apply Swing 2 if Torque non zero and is 2 or 3 axis joint
		if (JointInd < (AndroidPawn->NUM_2_PLUS_3_AXIS_JOINTS)) {
			if (CommandArray[ComInd] != 0) {
				float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
				SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, RotForce, 0.0f)), JointName, false);
			}
			ComInd++;

			// Apply Twist if Torque non zero and is 3 axis joint
			if (JointInd < AndroidPawn->NUM_3_AXIS_JOINTS) {
				if (CommandArray[ComInd] != 0) {
					float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
					SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(RotForce, 0.0f, 0.0f)), JointName, false);
				}
				ComInd++;
			}
		}
	}
>>>>>>> 255bf186d85d93ad1ff3df8cc29b8e552981befb
}
