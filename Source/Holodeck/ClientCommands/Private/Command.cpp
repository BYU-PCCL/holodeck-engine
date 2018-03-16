#include "Holodeck.h"
#include "Command.h"

UCommand::UCommand() {
	
}

UCommand* UCommand::Make() {
	return new UCommand();
}