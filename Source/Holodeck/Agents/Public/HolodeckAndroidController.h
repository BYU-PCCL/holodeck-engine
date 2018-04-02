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


protected:
	/**
	  * GetActionSpaceDimension
	  * The size of the action space for the android is 94.
	  * @return the size of the action space.
	  */
	int GetActionSpaceDimension() override { return 94; };

private:
	/**
	* SetJointConstraint
	* Sets the joint constraint. A helper function for ExecuteCommand.
	* @param Joint the joint to set the constraint on.
	* @param Swing1 the amount of swing1 to set.
	* @param Twist the amount of twist to set.
	* @param Swing2 the amount of swing2 to set.
	* @param Force the force to be applied.
	*/
	void SetJointConstraint(FName Joint, float Swing1, float Twist, float Swing2, float Force);

	void ApplyTorque(FName Joint, float Swing1, float Twist, float Swing2);

	USkeletalMeshComponent* SkeletalMeshComponent;
};
