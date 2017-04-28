#pragma once

#include "Components/SceneComponent.h"
#include "HolodeckSensor.h"
#include "IMUSensor.generated.h"

/**
  * An intertial measurement unit.
  * Returns a 1D numpy array of:
  * `[acceleration_x, acceleration_y, acceleration_z, velocity_roll, velocity_pitch, velocity_yaw]`
  */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOLODECK_API UIMUSensor : public UHolodeckSensor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIMUSensor();

	// Called when the game starts
	virtual void BeginPlay() override;

	FVector GetAccelerationVector();

	FVector GetAngularVelocityVector();

protected:
	virtual void SetDataType() override;

	virtual void TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void CalculateAccelerationVector(float DeltaTime);

	void CalculateAngularVelocityVector();

	UPrimitiveComponent* Parent;

	UWorld* World;
	AWorldSettings* WorldSettings;
	float WorldToMetersRatio;
	float WorldGravity;

	FVector VelocityThen;
	FVector VelocityNow;
	FRotator RotationNow;

	FVector LinearAccelerationVector;
	FVector AngularVelocityVector;

};