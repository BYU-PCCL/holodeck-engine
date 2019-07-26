// MIT License (c) 2019 BYU PCCL see LICENSE file

#pragma once

#include "GameFramework/Pawn.h"
#include "HolodeckAgent.h"
#include "TurtleAgent.generated.h"

UCLASS()
/**
* ATurtleAgent
* Inherits from the HolodeckAgent class
* On any tick this object will apply the given forces.
* Desired values must be set by a controller.
*/
class HOLODECK_API ATurtleAgent : public AHolodeckAgent
{
	GENERATED_BODY()

public:
	/**
	* Default Constructor.
	*/
	ATurtleAgent();

	void InitializeAgent() override;

	/**
	* Tick
	* Called each frame.
	* @param DeltaSeconds the time since the last tick.
	*/
	void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = UAVMesh)
		UStaticMeshComponent* RootMesh;

	unsigned int GetRawActionSizeInBytes() const override { return 2 * sizeof(float); };
	void* GetRawActionBuffer() const override { return (void*)CommandArray; };

private:
	/**
	* 0: ThrustToApply
	* 1: YawTorqueToApply
	*/
	float CommandArray[2];
};
