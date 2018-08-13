#include "Holodeck.h"
#include "RawControlScheme.h"

URawControlScheme::URawControlScheme(AHolodeckAgent* const ControlledAgent) {
	Agent = ControlledAgent;
	if (Agent == nullptr) {
		UE_LOG(LogHolodeck, Fatal, TEXT("Agent couldn't be set in control scheme!"));
	}
}

void URawControlScheme::Execute(void* const CommandArray, void* const InputCommand) const {
	memcpy(CommandArray, InputCommand, Agent->GetRawActionSizeInBytes());
}
