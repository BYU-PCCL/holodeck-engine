#pragma once

#include "Holodeck.h"

#include "Android.h"
#include "Command.h"
#include "SphereRobot.h"
#include "UAV.h"
#include "NavAgent.h"
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

	typedef std::map<std::string, UClass*> BlueprintMapType;
	static BlueprintMapType BlueprintMap;

	typedef AHolodeckAgent*(*SpawnFunctionType)(UClass* const, const FVector&, UWorld* const);
	typedef std::map<std::string, SpawnFunctionType> SpawnFunctionMapType;
	static SpawnFunctionMapType SpawnFunctionMap;
	
	/**
	  * SpawnAgent
	  * This is the template function that is used to spawn agents of the respective type.
	  * @param Blueprint The blueprint of the agent to spawn.
	  * @param Location The XYZ coordinate of where to spawn the agent.
	  * @param World The respective world in which to spawn the agent - usually should just be the world you are in.
	  * @return A pointer to the spawned AHolodeckAgent, or null if there was an error. 
	  */
	template<typename T>
	static AHolodeckAgent* SpawnAgent(UClass* const Blueprint, const FVector& Location, UWorld* const World);

	//This is forced inline because using FObjectFinder must be in the constructor.
	FORCEINLINE UClass* GetBlueprint(const FString&  Reference) {
		ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintVar(*Reference);
		if (BlueprintVar.Object)
			return (UClass*)BlueprintVar.Object->GeneratedClass;
		else
			UE_LOG(LogHolodeck, Warning, TEXT("SpawnAgentCommand unable to find blueprint for %s"), *Reference);
		return nullptr;
	}
};
