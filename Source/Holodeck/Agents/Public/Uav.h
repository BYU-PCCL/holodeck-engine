// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Pawn.h"
#include "HolodeckAgent.h"
#include "Uav.generated.h"

UCLASS()
/**
* AUav
* Inherits from the HolodeckAgent class
* On any tick this object will:
*     Calculate the forces to apply using PID controllers, desired values, and current values.
*     Apply the given forces. 
* Desired values must be set by a controller. 
*/
class HOLODECK_API AUav : public AHolodeckAgent
{
	GENERATED_BODY()
public:
	AUav();
	virtual void InitializeAgent() override;
	virtual void Tick(float DeltaSeconds) override;
	void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);
	void UpdateForcesAndMoments(float DeltaTime);
	void ApplyForces();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredAltitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredYawRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Desired)
		float DesiredRoll;
	UPROPERTY(BlueprintReadWrite, Category = UAVMesh)
		UStaticMeshComponent* RootMesh;

	//See HolodeckAgent.h for descriptions of these overriden functions

	float GetRollTorqueToApply() { return -CommandArray[0]; };
	float GetPitchTorqueToApply() { return -CommandArray[1]; };
	float GetYawTorqueToApply() { return CommandArray[2]; };
	float GetThrustToApply() { return CommandArray[3]; };

	unsigned int GetRawActionSizeInBytes() const override { return 4 * sizeof(float); };
	void* GetRawActionBuffer() const override { return (void*)CommandArray; };

protected:
	//See HolodeckAgent.h for descriptions of these overriden functions

private:
	/**
	  * 0: RollTorqueToApply
	  * 1: PitchTorqueToApply
	  * 2: YawTorqueToApply
	  * 3: ThrustToApply
	  */
	float CommandArray[4];

	FCalculateCustomPhysics OnCalculateCustomPhysics;

	/**
	  * InitializePIDControllers
	  */
	void InitializePIDControllers();
};
