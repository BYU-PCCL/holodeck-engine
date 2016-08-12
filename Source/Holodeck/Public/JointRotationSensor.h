#pragma once

#include "HolodeckPawnController.h"
#include "Components/SceneComponent.h"
#include "JointRotationSensor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UJointRotationSensor : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJointRotationSensor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	AHolodeckPawnController* Controller;

	TArray<FName> Bones;
	TArray<FName> ParentBones;
	USkeletalMeshComponent* SkeletalMeshComponent;	

	FString CastSingleJointRotationToString(FString jointName, bool swing1, bool twist, bool swing2);

	FString GetJointRotationVectorJSON();

	FString GetJointRotationsJSON();

	void PublishData();
};
