#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.generated.h"

UCLASS(ClassGroup = (Custom))
class HOLODECK_API UCommand : public UObject {
	GENERATED_BODY()

public:
	

	virtual void Execute() { check( 0 && "You must override UCommand::Execute" ); };


	//static std::unique_ptr<UCommand> CommandFactory(std::string Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters);

protected:

	std::vector<float> NumberParams;
	std::vector<std::string> StringParams;

	UCommand();
	UCommand(std::vector<float> Numberparams, std::vector<std::string> StringParams);

private:

};
