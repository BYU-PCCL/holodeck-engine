#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SpawnAgentCommand.generated.h"

UCLASS(ClassGroup = (Custom))
class HOLODECK_API USpawnAgentCommand : public UCommand {
	GENERATED_BODY()

public:
	void Execute() override;
	USpawnAgentCommand();
	USpawnAgentCommand(std::vector<float> NumberParams, std::vector<std::string> StringParams);

private:
	const FString UAV = "UAV";
	const FString SphereRobot = "SphereRobot";
	const FString Android = "Android";
};
