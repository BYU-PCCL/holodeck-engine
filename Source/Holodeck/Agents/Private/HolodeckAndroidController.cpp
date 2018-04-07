// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckAndroidController.h"

// Used to convert unreal torque unit (kg*cm^2/s^2) to (kg*m^2/s^2)
const float CM_TORQUE_TO_M_TORQUE = 10000;

AHolodeckAndroidController::AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Android Controller Initialized"));
}

AHolodeckAndroidController::~AHolodeckAndroidController() {}

void AHolodeckAndroidController::Possess(APawn* PawnPtr) {
	Super::Possess(PawnPtr);

	TArray<USkeletalMeshComponent*> Components;
	PawnPtr->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMesh = Components[0];
}

void AHolodeckAndroidController::ExecuteCommand() {

}

void AHolodeckAndroidController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(AndroidPawn == nullptr)
		AndroidPawn = Cast<AAndroid>(GetPawn());

	if (ActionBuffer != nullptr && AndroidPawn != nullptr) {
		CommandArray = static_cast<float*>(ActionBuffer);
		ApplyTorques();
	}
}

void AHolodeckAndroidController::ApplyTorques() {

	int ComInd = 0;

	for (int JointInd = 0; JointInd < AndroidPawn->NUM_JOINTS; JointInd++) {

		FName JointName = AndroidPawn->Joints[JointInd];

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
}