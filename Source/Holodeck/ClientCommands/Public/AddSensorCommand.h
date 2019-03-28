#pragma once

#include "Holodeck.h"

#include <map>
#include "CollisionSensor.h"
#include "IMUSensor.h"
#include "JointRotationSensor.h"
#include "LocationSensor.h"
#include "OrientationSensor.h"
#include "PressureSensor.h"
#include "RelativeSkeletalPositionSensor.h"
#include "RGBCamera.h"
#include "RotationSensor.h"
#include "VelocitySensor.h"
#include "ViewportCapture.h"
#include "DistanceTask.h"
#include "FollowTask.h"

#include "Command.h"
#include "AddSensorCommand.generated.h"

/**
* AddSensorCommand
* Command used to add a sensor to an agent
* Warning: This command is meant for initialization. Adding a sensor with the same name as a previously 
* removed sensor may cause problems. Use SetSensorEnabledCommand when possible. 
*
* StringParameters expect four arguments, the agent name, the socket name, sensor name, and sensor class.
*/
UCLASS()
class HOLODECK_API UAddSensorCommand : public UCommand
{
	GENERATED_BODY()

	typedef std::map<FString, UClass*> USensorMapType;

public:
	//See UCommand for the documentation of this overridden function.
	void Execute() override;

};
