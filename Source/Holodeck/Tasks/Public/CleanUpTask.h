// MIT License (c) 2019 BYU PCCL see LICENSE file

#pragma once

#include "CoreMinimal.h"
#include "TaskSensor.h"
#include "CleanUpTask.generated.h"

/**
* UCleanUpTask
* Inherits from the TaskSensor class.
* Initializes the clean up task in the world. This only works in the CleanUp world. 
* The reward is based on the number of pieces of trash placed in the trash can. 
* If the amount of trash in the trash can goes up, a reward is given by how much trash was added.
* If it goes down, a corresponding negative reward is given.
* If all the trash is in the trash can terminal is given.
*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HOLODECK_API UCleanUpTask : public UTaskSensor
{
	GENERATED_BODY()

public:

	/**
	* InitializeSensor
	* Sets up the class
	*/
	virtual void InitializeSensor() override;

	/**
	* Allows parameters to be set dynamically
	*/
	virtual void ParseSensorParms(FString ParmsJson) override;

protected:
	// Checkout HolodeckSensor.h for the documentation for this overridden function.
	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int PrevTick_TrashInCan = 0;
	int TotalTrash = 0;
};