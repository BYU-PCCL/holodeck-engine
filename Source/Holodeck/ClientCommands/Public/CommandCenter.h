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
	  * It also checks for commands that are in the json buffer. 
	  * If you write to the buffer, make sure to set the shouldreadbufferptr to true. 
	  * This should be called by UHolodeckGameMode.
	  * @param DeltaTime How much time has passed since the last tick.
	  */
	virtual void Tick(float DeltaTime);

	/**
	  *Init
	  * Always make sure to call this function after instantiating a CommandCenter
	  * It opens a channel for receiving commands in json format. It parses them and then issues the commands.
  	  * @param Server The holodeck server for the game.
  	  * @param GameMode The game mode for the instance. This pointer is needed for giving commands.
	  */
	void Init(UHolodeckServer* Server, AHolodeckGameMode* GameMode);

private:
	/**
	  *GetCommandBuffer
	  * Sets up the buffer used for pasing json commands to the command center. 
	  * also sets up the bool buffer. 
	  */
	virtual void GetCommandBuffer();
	/**
	  *ReadCommandBuffer
	  * Reads the buffer, parses the resulting json, and gives the commands to the command queue.
	  */
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

	/**
	  *PrintJson
	  * Reads the buffer, parses the resulting json, and gives the commands to the command queue.
	  */
	void PrintJson(JsonValue Input);
	void ExtractCommandsFromJson(JsonValue Input);
	void GetCommand(JsonValue Input);
};
