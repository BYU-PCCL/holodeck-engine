#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "HolodeckGameMode.h"
#include "DayTimeCommand.generated.h"

/**
* DayTimeCommand
* The command used to change the lighting and sky sphere in the world.
*
* StringParameters are expected to be empty.
* NumberParameters are expected to be an hour between 0-23 in military time.
*
* This command may not work properly if there is no skysphere or directional light in the world, but this should not cause a crash.
*/
UCLASS(ClassGroup = (Custom))
class HOLODECK_API UDayTimeCommand : public UCommand
{
	GENERATED_BODY()
	
	
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
	
private:
	int32 degrees_per_hour = 15; // the angle is 0 - 360 and corresponds to hours 0 - 24. This is the degrees for each hour (360/24)
	int32 hour_offset = 6; // hour should be increased by this value to sync with angle of lightsource 

	/**
	* GetAngle
	* Gets the required origin
	* @param hour The hour to convert to an angle. See the implementation of this 
	              function to see some corresponding values of hours to angles.
	* @return int32 The angle between 0 and 360.
	*/
	int32 GetAngle(int32 hour);
};
