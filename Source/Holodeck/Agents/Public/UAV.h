// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "SimplePID.h"
#include "GameFramework/Pawn.h"
#include "HolodeckAgentInterface.h"
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
class HOLODECK_API AUAV : public APawn, public IHolodeckAgentInterface
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool TeleportAndRotate(const FVector& NewLocation, FRotator NewRotation);
		virtual bool TeleportAndRotate_Implementation(const FVector& NewLocation, FRotator NewRotation) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool Teleport(const FVector& NewLocation);
		virtual bool Teleport_Implementation(const FVector& NewLocation) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool InitializeController();
		virtual bool InitializeController_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		FString GetAgentName();
		virtual FString GetAgentName_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetAgentName(const FString& Name);
		virtual bool SetAgentName_Implementation(const FString& Name) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		AHolodeckPawnController* GetHolodeckPawnController();
		virtual AHolodeckPawnController* GetHolodeckPawnController_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetHolodeckPawnController(AHolodeckPawnController* HolodeckController);
		virtual bool SetHolodeckPawnController_Implementation(AHolodeckPawnController* HolodeckController) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SpawnController();
		virtual bool SpawnController_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetTerminal(bool Terminal);
		virtual bool SetTerminal_Implementation(bool Terminal) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HolodeckAgent")
		bool SetReward(int Reward);
		virtual bool SetReward_Implementation(int Reward) override;

	// Must be set in the editor.
	UPROPERTY(EditAnywhere)
		FString AgentName;

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

	float* RewardPtr;
	bool* TerminalPtr;
	AHolodeckPawnController* PawnController;
};
