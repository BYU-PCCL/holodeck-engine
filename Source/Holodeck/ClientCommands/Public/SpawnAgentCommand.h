#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "Command.h"
#include "SphereRobot.h"
#include "UAV.h"
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
	static TSubclassOf<class AUAV> UAVBlueprint;
	static TSubclassOf<class AAndroid> AndroidBlueprint;
	static TSubclassOf<class ASphereRobot> SphereRobotBlueprint;
	static bool bFirstInstance;
};
