// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "SimplePID.h"
#include "GameFramework/Pawn.h"
#include "HolodeckAgent.h"
#include "UAV.generated.h"

UCLASS()
/**
* AUAV
* Inherits from the HolodeckAgent class
* On any tick this object will:
*     Calculate the forces to apply using PID controllers, desired values, and current values.
*     Apply the given forces. 
* Desired values must be set by a controller. 
*/
class HOLODECK_API AUAV : public AHolodeckAgent
{
	GENERATED_BODY()
public:
	AUAV();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);
	void UpdateForcesAndMoments(float DeltaTime);
	void ApplyForces();
	float UEUnitsToMeters(float ValueInUnrealUnits);
	FVector RotatorToEulerInZYX(FRotator quat);
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
	UPROPERTY(BlueprintReadWrite, Category = UAVMesh)
		float ThrustToApply;

	//See HolodeckAgent.h for descriptions of these overriden functions
	void SetHyperparameterAddress(float* Input) override;

protected:
	//See HolodeckAgent.h for descriptions of these overriden functions
	virtual const float* GetDefaultHyperparameters() const override;
	int GetHyperparameterCount() const override;

private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;
	// PID Controllers
	SimplePID RollController;
	SimplePID PitchController;
	SimplePID YawController;
	SimplePID AltitudeController;
	// TODO: move these out of the class and to arguments on ApplyForces, rename ApplyForces to ApplyTorquesAndForces
	float RollTorqueToApply;
	float PitchTorqueToApply;
	float YawTorqueToApply;
	// State
	float CurrentThrust;
	float CurrentRollTorque;
	float CurrentPitchTorque;
	float CurrentYawTorque;
	float CurrentPositionX;
	float CurrentPositionY;
	float CurrentPositionZ;
	float CurrentRoll;
	float CurrentPitch;
	float CurrentYaw;
	float CurrentGlobalVelocityZ;
	float CurrentYawRate;
	// Wind
	FVector Wind;
	const float* HyperparametersPointer;

	/**
	  * InitializePIDControllers
	  * Sets the PID controllers to point to the HyperparametersPointer array, with correct offsets for each variable of the respective controllers.
	  */
	void InitializePIDControllers();
};
