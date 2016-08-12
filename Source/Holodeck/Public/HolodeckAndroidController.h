#pragma once

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "HolodeckAndroidController.generated.h"

UCLASS()
class HOLODECK_API AHolodeckAndroidController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AHolodeckAndroidController();

	void Possess(APawn* pawn);
	void OnReceiveCommand(const FHolodeckAndroidCommand& Command);
	void OnReceiveConfiguration(const FHolodeckAndroidConfiguration& Configuration);
	void SetJointConstraint(FName joint, float swing1, float twist, float swing2, float force);

private:
	USkeletalMeshComponent* SkeletalMeshComponent;
};
