#pragma once

#include "Holodeck.h"

#include <vector>

#include "Command.generated.h"

UCLASS(ClassGroup = (Custom), abstract)
class HOLODECK_API UCommand : public UObject {
	GENERATED_BODY()

public:
	UCommand();

	virtual void Execute() { check( 0 && "You must override UCommand::Execute" ); };


	static UCommand* Make();

private:

	std::vector<int> IntParams;
	std::vector<std::string> StringParams;

};
