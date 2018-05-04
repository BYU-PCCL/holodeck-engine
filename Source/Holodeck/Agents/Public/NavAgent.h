// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Holodeck.h"
#include "HolodeckAgentInterface.h"

#include "NavAgent.generated.h"

UCLASS()
class HOLODECK_API ANavAgent : public APawn, public IHolodeckAgentInterface
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor.
	*/
	ANavAgent();

	/**
	* BeginPlay
	* Called when the game starts.
	* Registers the reward and terminal signals.
	*/
	void BeginPlay() override;

	/**
	* Tick
	* Called each frame.
	* @param DeltaSeconds the time since the last tick.
	*/
	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Holodeck")
	FVector GetTarget();

	UFUNCTION(BlueprintCallable, Category = "Holodeck")
	void SetTarget(float x, float y, float z);

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
		virtual bool SpawnController_Implementation();

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
	FVector Target;
	float* RewardPtr;
	bool* TerminalPtr;
	AHolodeckPawnController* PawnController;

};
