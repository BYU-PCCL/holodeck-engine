#pragma once

#include "HolodeckPawnController.h"
#include "HolodeckSensor.h"
#include "JointRotationSensor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UJointRotationSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJointRotationSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
	virtual FString GetDataKey() override;
	virtual int GetDataLength() override;

	// Called every frame
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FName> Bones;
	TArray<FName> ParentBones;
	USkeletalMeshComponent* SkeletalMeshComponent;	

	FString CastSingleJointRotationToString(FString jointName, bool swing1, bool twist, bool swing2);

	FString GetJointRotationVectorJSON();

	FString GetJointRotationsJSON();

	void PublishData();
};
