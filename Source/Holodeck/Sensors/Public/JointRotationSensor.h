#pragma once

#include "Holodeck.h"
#include "Android.h"

#include "HolodeckPawnController.h"
#include "HolodeckSensor.h"
#include "PhysicsEngine/ConstraintInstance.h"

#include "JointRotationSensor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UJointRotationSensor : public UHolodeckSensor {
	GENERATED_BODY()

public:	
	/**
	  * Default Constructor.
	  */
	UJointRotationSensor();

	/**
	  * BeginPlay
	  * Called when the games starts.
	  */
	void BeginPlay() override;
	
protected:
	// See HolodeckSensor for information on these classes.
	virtual FString GetDataKey() override { return "JointRotationSensor"; };
	virtual int GetNumItems() override { return 94; };
	virtual int GetItemSize() override { return sizeof(float); };
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FName> Bones;
	TArray<FName> ParentBones;
	USkeletalMeshComponent* SkeletalMeshComponent;	
	AAndroid* Android;

	/**
	  * AddJointRotationToBuffer
	  * Adds a certain joint rotation to the buffer.
	  * @param JointName the name of the joint to add.
	  * @param Swing1 true to insert the swing1 value.
	  * @param Swing2 true to insert the swing2 value.
	  * @param Twist true to insert the twist value.
	  * @param Data a pointer into the data buffer at the point the data should be inserted
	  * @return a pointer to the next position in the buffer.
	  */
	float* AddJointRotationToBuffer(FString JointName, bool Swing1, bool Swing2, bool Twist, float* Data);
};
