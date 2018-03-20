#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "Command.h"
#include "SphereRobot.h"
#include "UAV.h"
#include "SpawnAgentCommand.generated.h"

/**
  * SpawnAgentCommand
  * The command used to spawn agents in the world.
  * 
  * StringParameters are expected to be AgentType, and then Name.
  * NumberParameters are expected to be X, Y, then Z coords of the location to spawn at.
  * 
  * To enable it to spawn other agents you will need to: 
  * 1. Add its respective variables to the private variables of the class.
  * 2. Add its reference to the const static variable in the cpp file.
  *		-Right click on the blueprint in the content browser of the unreal editor, and click on Copy reference. Paste that as the value of the variable.
  * 3. Follow the pattern already in place for initializing the AgentBlueprint variable in the constructor. 
  * 4. Add the if statement to spawn the agent and its controller towards the bottom of the execute function. 
  */
UCLASS(ClassGroup = (Custom))
class HOLODECK_API USpawnAgentCommand : public UCommand {
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;

	/**
	  * USpawnAgentCommand
	  * Default constructor.
	  * This is where the static blueprint variables will be found the first time a
	  * USpawnAgentCommand is instantiated. 
	  */
	USpawnAgentCommand();

private:
	//These are the names of the agents that are spawnable
	const FString UAV = "UAV";
	const FString SphereRobot = "SphereRobot";
	const FString Android = "Android";
	//These are their blueprints, and are correctly initialized the first time a SpawnAgentCommand is instantiated. 
	static TSubclassOf<class AUAV> UAVBlueprint;
	static TSubclassOf<class AAndroid> AndroidBlueprint;
	static TSubclassOf<class ASphereRobot> SphereRobotBlueprint;
	static bool bFirstInstance;
};
