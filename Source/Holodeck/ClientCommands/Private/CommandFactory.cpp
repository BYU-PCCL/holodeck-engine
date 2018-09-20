#include "Holodeck.h"
#include "CommandFactory.h"
#include "HolodeckGameMode.h"

const static std::string SPAWN_AGENT = "SpawnAgent";

UCommand* UCommandFactory::MakeCommand(const std::string& Name, const std::vector<float>& NumberParameters, const std::vector<std::string>& StringParameters, AActor* ParameterGameMode) {
	static UCommandMapType CommandMap = { { "SpawnAgent", &CreateInstance<USpawnAgentCommand>},
										  { "ChangeFogDensity", &CreateInstance<UChangeFogDensityCommand> },
										  { "DayTime", &CreateInstance<UDayTimeCommand> },
										  { "SetWeather", &CreateInstance<USetWeatherCommand> },
										  { "DayCycle", &CreateInstance<UDayCycleCommand> },
										  { "TeleportCamera", &CreateInstance<UTeleportCameraCommand> } };
	UCommand*(*CreateCommandFunction)()  = CommandMap[Name];
	UCommand* ToReturn = nullptr;
	if (CreateCommandFunction) {
		ToReturn = CreateCommandFunction();
		ToReturn->Init(NumberParameters, StringParameters, ParameterGameMode);
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("CommandFactory failed to make command:  %s"), UTF8_TO_TCHAR(Name.c_str()));
	}
	return ToReturn;
}
