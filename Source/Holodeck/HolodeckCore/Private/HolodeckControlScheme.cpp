#include "Holodeck.h"

#include "HolodeckControlScheme.h"


void UHolodeckControlScheme::Execute(void* const CommandArray, void* const InputCommand) const {
	check(0 && "You must override Execute");
}

unsigned int UHolodeckControlScheme::GetControlSchemeByteSize() const {
	check(0 && "You must override GetControlSchemeByteSize");
}
