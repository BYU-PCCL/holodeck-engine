#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.generated.h"
//class AHolodeckGameMode

UCLASS(ClassGroup = (Custom), abstract)
class HOLODECK_API UCommand : public UObject {
	GENERATED_BODY()

public:


	virtual void Execute() { check( 0 && "You must override UCommand::Execute" ); };
	UCommand();
	UCommand(std::vector<float> NumberParameters, std::vector<std::string> StringParameters);
	virtual void Init(std::vector<float> NumberParameters, std::vector<std::string> StringParameters/*, AHolodeckGameMode* GameMode*/);
	//static std::unique_ptr<UCommand> CommandFactory(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters);

protected:

	std::vector<float> NumberParams;
	std::vector<std::string> StringParams;
	//AHolodeckGameMode* GameMode;
	

private:

};
