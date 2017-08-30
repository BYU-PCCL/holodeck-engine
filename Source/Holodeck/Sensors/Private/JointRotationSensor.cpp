#include "Holodeck.h"
#include "JointRotationSensor.h"

UJointRotationSensor::UJointRotationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UJointRotationSensor::BeginPlay() {
	Super::BeginPlay();

	AActor* Android = this->GetOwner();
	TArray<USkeletalMeshComponent*> Components;
	Android->GetComponents<USkeletalMeshComponent>(Components);
	SkeletalMeshComponent = Components[0];

	Bones = SkeletalMeshComponent->GetAllSocketNames();
	ParentBones.Reserve(Bones.Num());
	for (int i = 0; i < Bones.Num(); i++) {
		ParentBones.Insert(SkeletalMeshComponent->GetParentBone(Bones[i]), i);
	}
}

void UJointRotationSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	float* FloatBuffer = static_cast<float*>(Buffer);

	FloatBuffer = AddJointRotationToBuffer("head", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("neck_01", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("spine_02", true, true, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("spine_01", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("upperarm_l", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("lowerarm_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("hand_l", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_01_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_02_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_03_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_01_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_02_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_03_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_01_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_02_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_03_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_01_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_02_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_03_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_01_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_02_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_03_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("upperarm_r", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("lowerarm_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("hand_r", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_01_r", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_02_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thumb_03_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_01_r", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_02_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("index_03_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_01_r", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_02_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("middle_03_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_01_r", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_02_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ring_03_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_01_r", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_02_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("pinky_03_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thigh_l", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("calf_l", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("foot_l", true, false, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("ball_l", true, true, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("thigh_r", true, true, true, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("calf_r", true, false, false, FloatBuffer);
	FloatBuffer = AddJointRotationToBuffer("foot_r", true, false, true, FloatBuffer);
	AddJointRotationToBuffer("ball_r", true, true, false, FloatBuffer);
}

float* UJointRotationSensor::AddJointRotationToBuffer(FString JointName, bool Swing1, bool Twist, bool Swing2, float* Data) {
	FConstraintInstance* Constraint = SkeletalMeshComponent->FindConstraintInstance(FName(*JointName));
	if (Swing1) {
		*Data = Constraint->GetCurrentSwing1();
		Data += 1;
	}
	if (Twist) {
		*Data = Constraint->GetCurrentTwist();
		Data += 1;
	}
	if (Swing2) {
		*Data = Constraint->GetCurrentSwing2();
		Data += 1;
	}

	return Data;
}
