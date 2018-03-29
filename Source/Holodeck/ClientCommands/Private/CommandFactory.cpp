#include "Holodeck.h"
#include "CommandFactory.h"
#include "HolodeckGameMode.h"

const static std::string SPAWN_AGENT = "SpawnAgent";

UCommand* UCommandFactory::MakeCommand(const std::string& Name, std::vector<float> NumberParameters, std::vector<std::string> StringParameters, AActor* ParameterGameMode) {
	static UCommandMapType CommandMap = { { "SpawnAgent", &CreateInstance<USpawnAgentCommand>} };
	UCommand*(*CreateCommandFunction)()  = CommandMap[Name];
	UCommand* ToReturn = nullptr;
	if (CreateCommandFunction) {
		ToReturn = CreateCommandFunction();
		ToReturn->Init(NumberParameters, StringParameters, ParameterGameMode);
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("CommandFactory failed to make command:  %s"), UTF8_TO_TCHAR(Name.c_str()));
	}
	return ToReturn;
}


