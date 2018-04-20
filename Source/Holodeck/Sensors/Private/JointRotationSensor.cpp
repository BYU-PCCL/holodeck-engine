#include "Holodeck.h"
#include "JointRotationSensor.h"

UJointRotationSensor::UJointRotationSensor() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UJointRotationSensor::BeginPlay() {
	Super::BeginPlay();

	Android = Cast<AAndroid>(this->GetOwner());
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

	const FName* Joints = Android->Joints;

	for (int JointInd = 0; JointInd < Android->NUM_JOINTS; JointInd++) {

		FString JointName = Joints[JointInd].ToString();

		if (JointInd < Android->NUM_3_AXIS_JOINTS) {
			FloatBuffer = AddJointRotationToBuffer(JointName, true, true, true, FloatBuffer);
		}
		else if (JointInd < (Android->NUM_2_PLUS_3_AXIS_JOINTS)) {
			FloatBuffer = AddJointRotationToBuffer(JointName, true, true, false, FloatBuffer);
		}
		else {
			FloatBuffer = AddJointRotationToBuffer(JointName, true, false, false, FloatBuffer);
		}
	}
}

float* UJointRotationSensor::AddJointRotationToBuffer(FString JointName, bool Swing1, bool Swing2, bool Twist, float* Data) {
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