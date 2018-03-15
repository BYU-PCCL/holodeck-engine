#pragma once

#include "Holodeck.h"

#include "Command.h"
#include "SpawnUAVCommand.generated.h"

UCLASS(ClassGroup = (Custom))
class HOLODECK_API USpawnUAVCommand : public UCommand {
	GENERATED_BODY()

public:
	USpawnUAVCommand();
	USpawnUAVCommand(FVector Location);


	virtual void Execute() override;

private:

	FVector Location;
	static FString GetNewName();
};
