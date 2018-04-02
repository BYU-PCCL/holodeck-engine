// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "Android.h"

// Used to convert unreal torque unit (kg*cm^2/s^2) to (kg*m^2/s^2)
const float CM_TORQUE_TO_M_TORQUE = 10000;

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

	const FName* Joints = GetJoints();
	int ComInd = 0;

	for (int JointInd = 0; JointInd < NUM_JOINTS; JointInd++) {

		FName JointName = Joints[JointInd];
		UE_LOG(LogTemp, Warning, TEXT("On Joint %i"),JointInd);

		// Get rotation of that socket
		FTransform JointTransform = SkeletalMesh->GetSocketTransform(Joints[JointInd]);
		FQuat RotQuat = SkeletalMesh->GetSocketQuaternion(Joints[JointInd]);

		UE_LOG(LogTemp, Warning, TEXT("Swing1 Com is %f"), CommandArray[ComInd]);
		// Apply Swing 1 Torque if non zero
		if (CommandArray[ComInd] != 0) {
			float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
			SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, 0.0f, RotForce)), JointName, false);

			UE_LOG(LogTemp, Warning, TEXT("Added Swing 1"));
			//FVector LocalRotVector = FVector(0.0f, RotForce, 0.0f);
			//FVector Swing1TorqVec = JointTransform.TransformVector(LocalRotVector);
			//SkeletalMesh->AddTorque(Swing1TorqVec, Joints[JointInd], false);
		}
		ComInd++;
		UE_LOG(LogTemp, Warning, TEXT("Swing2 Com is %f"), CommandArray[ComInd]);

		UE_LOG(LogTemp, Warning, TEXT("Threshold 1 is %d"), (NUM_3_AXIS_JOINTS + NUM_2_AXIS_JOINTS));

		// Apply Swing 2 if Torque non zero and is 2 or 3 axis joint
		if (JointInd < (NUM_3_AXIS_JOINTS + NUM_2_AXIS_JOINTS)){
			UE_LOG(LogTemp, Warning, TEXT("Entered 2nd if"));
			if (CommandArray[ComInd] != 0) {
				float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
				SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(0.0f, RotForce, 0.0f)), JointName, false);
				UE_LOG(LogTemp, Warning, TEXT("Added Swing 2"));
				//FVector LocalRotVector = FVector(0.0f, 0.0f, RotForce);
				//FVector Swing1TorqVec = JointTransform.TransformVector(LocalRotVector);
				//SkeletalMesh->AddTorque(Swing1TorqVec, Joints[JointInd], false);
			}
			ComInd++;
			UE_LOG(LogTemp, Warning, TEXT("Twist Com is %f"), CommandArray[ComInd]);
		}

		UE_LOG(LogTemp, Warning, TEXT("Threshold 2 is %d"), (NUM_3_AXIS_JOINTS ));
		// Apply Twist if Torque non zero and is 3 axis joint
		if (JointInd < NUM_3_AXIS_JOINTS) {
			UE_LOG(LogTemp, Warning, TEXT("Entered 3rd if"));
			if (CommandArray[ComInd] != 0) {
				float RotForce = CommandArray[ComInd] * CM_TORQUE_TO_M_TORQUE;
				SkeletalMesh->AddTorque(RotQuat.RotateVector(FVector(RotForce, 0.0f, 0.0f)), JointName, false);
				UE_LOG(LogTemp, Warning, TEXT("Added Swing 3"));
				//FVector LocalRotVector = FVector(RotForce, 0.0f, 0.0f);
				//FVector Swing1TorqVec = JointTransform.TransformVector(LocalRotVector);
				//SkeletalMesh->AddTorque(Swing1TorqVec, Joints[JointInd], false);
			}
			ComInd++;
		}
	}
}

const FName* AAndroid::GetJoints()
{
	static FName Joints[] = { 
		
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
	return Joints;
}