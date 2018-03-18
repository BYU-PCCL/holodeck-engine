#pragma once

#include "Holodeck.h"

#include <tuple>
#include <vector>

#include "Command.h"
#include "CommandFactory.h"
#include "gason.h"
#include "HolodeckServer.h"
#include "CommandCenter.generated.h"

class AHolodeckGameMode; //forward declare to avoid circular dependency. 

UCLASS(ClassGroup = (Custom))
class HOLODECK_API UCommandCenter : public UObject {
	GENERATED_BODY()

		UCommandCenter();

public:
	/**
	  * GiveCommand
	  * It is used to execute whatever queued up commands there are.
	  * @param InputCommand A unique_ptr to a command. The given pointer will be null after this function call.
	  */
	virtual void GiveCommand(UCommand* &InputCommand);

	/**
	  *Tick
	  * It is used to execute whatever queued up commands there are.
	  * This should be called by UHolodeckGameMode
	  * @param DeltaTime How much time has passed since the last tick.
	  */
	virtual void Tick(float DeltaTime);

	

	void Init(UHolodeckServer* Server, AHolodeckGameMode* GameMode);

	//double sum_and_print(JsonValue o);

private:
	virtual void GetCommandBuffer();
	int ReadCommandBuffer();
	
	TArray<UCommand*> Commands;
	char* Buffer;
	void* ShouldReadBufferPtr;
	UHolodeckServer* Server;
	AHolodeckGameMode* GameMode;
	FString BUFFER_NAME = "command_buffer";
	FString BUFFER_SHOULD_READ_NAME = "command_bool";
	int BUFFER_SHOULD_READ_SIZE = 1;
	int BUFFER_SIZE = 1048576; 

	void PrintJson(JsonValue Input);
	void ExtractCommandsFromJson(JsonValue Input);
	void GetCommand(JsonValue Input);
	//std::tuple<std::string, std::string> GetParameter(JsonValue Input);


};
