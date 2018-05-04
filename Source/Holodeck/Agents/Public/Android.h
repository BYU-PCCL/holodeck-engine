// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "GameFramework/Pawn.h"
#include "HolodeckAgentInterface.h"
#include "PressureSensor.h"

#include "Android.generated.h"

UCLASS()
class HOLODECK_API AAndroid : public APawn, public IHolodeckAgentInterface
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

		/**
		* ApplyTorques
		* Applies torques for that tick on each joint with a force/direction
		* corresponding to the values in the command array
		*/
		void ApplyTorques();

		bool bCollisionsAreVisible;
		float* RewardPtr;
		bool* TerminalPtr;
		AHolodeckPawnController* PawnController;
};
