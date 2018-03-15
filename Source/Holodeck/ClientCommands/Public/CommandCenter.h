#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.h"
#include "gason.h"
#include "HolodeckServer.h"
#include "CommandCenter.generated.h"

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
	virtual void GiveCommand(std::unique_ptr<UCommand> &InputCommand);

	virtual void GiveCommands(JsonValue Value);

	/**
	  *Tick
	  * It is used to execute whatever queued up commands there are.
	  * This should be called by UHolodeckGameMode
	  * @param DeltaTime How much time has passed since the last tick.
	  */
	virtual void Tick(float DeltaTime);

	

	void Init(UHolodeckServer* Server);

	//double sum_and_print(JsonValue o);

private:
	virtual void GetCommandBuffer();
	int ReadCommandBuffer();
	
	std::vector<std::unique_ptr<UCommand>> Commands;
	char* Buffer;
	void* ShouldReadBufferPtr;
	UHolodeckServer* Server;
	std::string BUFFER_NAME = "CommandBuffer";
	std::string BUFFER_SHOULD_READ_NAME = "CommandBufferBool";
	int BUFFER_SHOULD_READ_SIZE = 1;
	int BUFFER_SIZE = 1048576; 

	void PrintJson(JsonValue Input);

};
