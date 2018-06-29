// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckAgent.h"
#include "PressureSensor.h"

#include "Android.generated.h"

UCLASS()
class HOLODECK_API AAndroid : public AHolodeckAgent
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor
	*/
	AAndroid();

	static const int NUM_JOINTS = 48;
	static const int NUM_3_AXIS_JOINTS = 18;
	static const int NUM_2_AXIS_JOINTS = 10;
	static const int NUM_1_AXIS_JOINTS = 20;
	static const int NUM_2_PLUS_3_AXIS_JOINTS = 28;
	static const int TOTAL_DOF = NUM_3_AXIS_JOINTS * 3 + NUM_2_AXIS_JOINTS * 2 + NUM_1_AXIS_JOINTS; // 94 DOF in total

	const static FName Joints[];

	/**
	* Called when the game starts.
	*/
	virtual void BeginPlay() override;

	/**
	* Tick
	* Called every frame.
	* @param DeltaSeconds the time since the last tick.
	*/
	void Tick(float DeltaSeconds) override;

	/**
	* TODO(joshgreaves) : Explain this function properly.
	* NotifyHit
	* Used for the pressure sensor.
	*/
	void NotifyHit(UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	//Decal material. This is used to show collisions on the Android. It is to be left blank and is set programmatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
		UMaterial* CollisionDecalMaterial;

	/**
	* SetCollisionsVisible
	* Sets whether collisions with the Android can be seen.
	* @param Visible true to be seen.
	*/
	void SetCollisionsVisible(bool Visible);

	/**
	* GetJoints
	* Gets pointer to constant array of FName joints
	* @return array of FName corresponding to android joint names
	*/
	const FName* GetJoints();

	/**
	* GetCollisionsVisible
	* @return true if collisions are visible.
	*/
	bool GetCollisionsVisible();

	float* CommandArray;

	UPROPERTY(BlueprintReadWrite, Category = AndroidMesh)
		USkeletalMeshComponent* SkeletalMesh;

private:
	bool bCollisionsAreVisible;

	/**
	* ApplyTorques
	* Applies torques for that tick on each joint with a force/direction
	* corresponding to the values in the command array
	*/
	void ApplyTorques();
};
