#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SpawnAgentCommand.generated.h"

/**
  * SpawnAgentCommand
  * The command used to spawn agents in the world.
  * 
  * StringParameters are expected to be AgentType, and then Name.
  * NumberParameters are expected to be X, Y, then Z coords of the location to spawn at.
  * 
  * To enable it to spawn other agents you will need to: 
  * 1. Add its respective variables to both of the maps in the constructor. 
  * 2. Add its reference to the const static variable in the cpp file.
  *		-Right click on the blueprint in the content browser of the unreal editor, and click on Copy reference. Paste that as the value of the variable.
  */
UCLASS(ClassGroup = (Custom), Blueprintable)
class HOLODECK_API USpawnAgentCommand : public UCommand {
	GENERATED_BODY()
public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
