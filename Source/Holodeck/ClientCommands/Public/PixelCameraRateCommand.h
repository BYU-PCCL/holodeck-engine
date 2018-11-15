#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "PixelCameraRateCommand.generated.h"

/**
* PixelCameraRateCommand
* 
*/
UCLASS(ClassGroup = (Custom))
class HOLODECK_API UPixelCameraRateCommand : public UCommand {
	GENERATED_BODY()
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
