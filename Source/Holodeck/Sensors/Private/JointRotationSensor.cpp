#include "Holodeck.h"
#include "JointRotationSensor.h"
#include <iostream>
#include <string>

using namespace std;

// Sets default values for this component's properties
UJointRotationSensor::UJointRotationSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. 
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UJointRotationSensor::BeginPlay() {
	Super::BeginPlay();

	//set skeletal mesh component
	AActor* Android = this->GetOwner();
	TArray<USkeletalMeshComponent*> components;
	Android->GetComponents<USkeletalMeshComponent>(components);
	SkeletalMeshComponent = components[0];

	//Save arrays of bones and parent bones
	Bones = SkeletalMeshComponent->GetAllSocketNames();
	ParentBones.Reserve(Bones.Num());
	for (int i = 0; i < Bones.Num(); i++) {
		ParentBones.Insert(SkeletalMeshComponent->GetParentBone(Bones[i]), i);
	}
}

FString UJointRotationSensor::GetDataKey() {
	return "JointRotationSensor";
}

int UJointRotationSensor::GetDataLength() {
	return 1000;
}

// Called every frame
void UJointRotationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	//ResultData.Data = GetJointRotationVectorJSON();
}

FString UJointRotationSensor::GetJointRotationVectorJSON() {

	FString smallerResult = "[";

	smallerResult += CastSingleJointRotationToString("head", true, true, true);
	smallerResult += CastSingleJointRotationToString("neck_01", true, false, false);
	smallerResult += CastSingleJointRotationToString("spine_02", true, true, false);
	smallerResult += CastSingleJointRotationToString("spine_01", true, true, true);
	smallerResult += CastSingleJointRotationToString("upperarm_l", true, true, true);
	smallerResult += CastSingleJointRotationToString("lowerarm_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("hand_l", true, true, true);
	smallerResult += CastSingleJointRotationToString("thumb_01_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("thumb_02_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("thumb_03_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("index_01_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("index_02_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("index_03_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("middle_01_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("middle_02_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("middle_03_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("ring_01_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("ring_02_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("ring_03_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("pinky_01_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("pinky_02_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("pinky_03_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("upperarm_r", true, true, true);
	smallerResult += CastSingleJointRotationToString("lowerarm_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("hand_r", true, true, true);
	smallerResult += CastSingleJointRotationToString("thumb_01_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("thumb_02_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("thumb_03_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("index_01_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("index_02_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("index_03_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("middle_01_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("middle_02_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("middle_03_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("ring_01_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("ring_02_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("ring_03_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("pinky_01_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("pinky_02_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("pinky_03_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("thigh_l", true, true, true);
	smallerResult += CastSingleJointRotationToString("calf_l", true, false, false);
	smallerResult += CastSingleJointRotationToString("foot_l", true, false, true);
	smallerResult += CastSingleJointRotationToString("ball_l", true, true, false);
	smallerResult += CastSingleJointRotationToString("thigh_r", true, true, true);
	smallerResult += CastSingleJointRotationToString("calf_r", true, false, false);
	smallerResult += CastSingleJointRotationToString("foot_r", true, false, true);
	smallerResult += CastSingleJointRotationToString("ball_r", true, true, false);

	// Remove last comma
	smallerResult.RemoveAt(smallerResult.Len() - 1);
	smallerResult += "]";

	return smallerResult;
}


/* Output ALL of the data,
*  as opposed to just the relevant information. The code with the
*  smaller result is what we will most often use.
*/
FString UJointRotationSensor::GetJointRotationsJSON() {
	/*
	FString result = "[";

	FName BoneName;

	FConstraintInstance* Constraint;// = SkeletalMeshComponent->FindConstraintInstance(FName(*ConstraintCommand.Bone);

	FString swing1;
	FString twist;
	FString swing2;
	FString toAppend;

	vector<float> angleVector;

	//start at 2 to ignore root and pelvis which has no swing1, swing2, or twist.
	for (int i = 2; i < Bones.Num(); i++) {

		BoneName = FName(Bones[i]);

		Constraint = SkeletalMeshComponent->FindConstraintInstance(BoneName);

		if (Constraint != NULL) {

			//result += "{\"Bone\":\"" + Bones[i].ToString() + "\",";

			swing1 = FString::SanitizeFloat(Constraint->GetCurrentSwing1());
			twist = FString::SanitizeFloat(Constraint->GetCurrentTwist());
			swing2 = FString::SanitizeFloat(Constraint->GetCurrentSwing2());

			toAppend = (swing1 + "," + twist + "," + swing2 + ",");
			result += toAppend;
		}
	}
	result.RemoveAt(result.Len() - 1);
	result += "]";
	return result;*/
	return TEXT("");
}

FString UJointRotationSensor::CastSingleJointRotationToString(FString jointName, bool swing1, bool twist, bool swing2) {
	/*FConstraintInstance* Constraint = SkeletalMeshComponent->FindConstraintInstance(FName(*jointName));
	FString result;
	if (swing1) {
		result += FString::SanitizeFloat(Constraint->GetCurrentSwing1());
		result += ",";
	}
	if (twist) {
		result += FString::SanitizeFloat(Constraint->GetCurrentTwist());
		result += ",";
	}
	if (swing2) {
		result += FString::SanitizeFloat(Constraint->GetCurrentSwing2());
		result += ",";
	}
	return result;;*/
	return TEXT("");
}
