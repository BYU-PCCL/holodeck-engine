#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "HolodeckPawnController.h"
#include "PhysicsEngine/ConstraintInstance.h"

#include "HolodeckAndroidController.generated.h"

UCLASS()
class HOLODECK_API AHolodeckAndroidController : public AHolodeckPawnController
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AHolodeckAndroidController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	  * Default Destructor
	  */
	~AHolodeckAndroidController();

	/**
	  * Possess
	  * Called when the controller possesses the pawn.
	  * @param Pawn the pawn being possessed.
	  */
	void Possess(APawn* Pawn) override;

	/**
	  * ExecuteCommand
	  * Executes the commands sent from the client.
	  */
	void ExecuteCommand() override;

	/**
	* Tick
	* Called every frame.
	* @param DeltaSeconds the time since the last tick.
	*/
	void Tick(float DeltaSeconds) override;


protected:
	/**
	  * GetActionSpaceDimension
	  * The size of the action space for the android is 94.
	  * @return the size of the action space.
	  */
	int GetActionSpaceDimension() override { return 94; };

private:

	/**
	* ApplyTorques
	* Applies torques for that tick on each joint with a force/direction
	* corresponding to the values in the command array
	*/
	void ApplyTorques();

	USkeletalMeshComponent* SkeletalMesh;

	AAndroid* AndroidPawn;

	float* CommandArray;
};
