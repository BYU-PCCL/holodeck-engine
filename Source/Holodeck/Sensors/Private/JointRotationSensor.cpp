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

int UJointRotationSensor::GetNumItems() {
	return 79;
}

int UJointRotationSensor::GetItemSize() {
	return sizeof(float);
}

// Called every frame
void UJointRotationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	float* float_buffer = static_cast<float*>(buffer);

	float_buffer = AddJointRotationToBuffer("head", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("neck_01", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("spine_02", true, true, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("spine_01", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("upperarm_l", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("lowerarm_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("hand_l", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_01_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_02_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_03_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_01_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_02_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_03_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_01_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_02_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_03_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_01_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_02_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_03_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_01_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_02_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_03_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("upperarm_r", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("lowerarm_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("hand_r", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_01_r", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_02_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("thumb_03_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_01_r", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_02_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("index_03_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_01_r", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_02_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("middle_03_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_01_r", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_02_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("ring_03_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_01_r", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_02_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("pinky_03_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("thigh_l", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("calf_l", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("foot_l", true, false, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("ball_l", true, true, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("thigh_r", true, true, true, float_buffer);
	float_buffer = AddJointRotationToBuffer("calf_r", true, false, false, float_buffer);
	float_buffer = AddJointRotationToBuffer("foot_r", true, false, true, float_buffer);
	AddJointRotationToBuffer("ball_r", true, true, false, float_buffer);
}

float* UJointRotationSensor::AddJointRotationToBuffer(FString jointName, bool swing1, bool twist, bool swing2, float* data) {
	FConstraintInstance* Constraint = SkeletalMeshComponent->FindConstraintInstance(FName(*jointName));
	if (swing1) {
		*data = Constraint->GetCurrentSwing1();
		data += 1;
	}
	if (twist) {
		*data = Constraint->GetCurrentTwist();
		data += 1;
	}
	if (swing2) {
		*data = Constraint->GetCurrentSwing2();
		data += 1;
	}

	return data;
}
