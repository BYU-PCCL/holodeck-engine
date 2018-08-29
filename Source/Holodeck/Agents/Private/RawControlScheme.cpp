#include "Holodeck.h"
#include "RawControlScheme.h"


URawControlScheme::URawControlScheme(const FObjectInitializer& ObjectInitializer) :
		Super(ObjectInitializer) {}

URawControlScheme::URawControlScheme(AHolodeckAgentInterface* const ControlledAgent) {
	Agent = ControlledAgent;
	if (Agent == nullptr) {
		UE_LOG(LogHolodeck, Fatal, TEXT("Agent couldn't be set in control scheme!"));
	}
}

void URawControlScheme::Execute(void* const CommandArray, void* const InputCommand) {
	memcpy(CommandArray, InputCommand, Agent->GetRawActionSizeInBytes());
}
