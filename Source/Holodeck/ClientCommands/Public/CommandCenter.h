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

 /**
   * UCommandCenter
   * It subscribes a memory space to receive commands from the client binding.
   * It expects the memory to be formatted in UTF-8 JSON.
   * When creating a new commandcenter object, you must call the init after or it will not function. 
   * It requires a valid pointer to the server to be initialized, and the currrent gamemode object to continue running correctly. 
   */
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
	  * @return the status of the read/parse
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
	  * PrintJson
	  * Used solely for debugging. It is accurate and prints exactly how the json is structured.
	  * @param Input The JsonValue to print
	  */
	void PrintJson(JsonValue Input);

	/**
	  * ExctractCommandsFromJson
	  * Reads the buffer, parses the resulting json, and gives the commands to the command queue.
	  * It will not succeed if the json object is not in the ofrmat expected. 
	  * At a lower level, it calls GetCommand on every json object that in an array expected to be a command.
	  * @param Input the Json to extract commands from.
	  */
	void ExtractCommandsFromJson(JsonValue Input);

	/**
	  * GetCommand
	  * Traverses a specific json object that is a command.
	  * Separates the parameters into string parameters and number parameters, then pushes the command to the Commands array. 
	  */
	void GetCommand(JsonValue Input);
};
