#include "Holodeck.h"
#include "AndroidControlSchemeMaxTorque.h"




UAndroidControlSchemeMaxTorque::UAndroidControlSchemeMaxTorque(const FObjectInitializer& ObjectInitializer) :
		Super(ObjectInitializer) {}

void UAndroidControlSchemeMaxTorque::Execute(void* const CommandArray, void* const InputCommand, float DeltaSeconds) {
    float* CommandArrayFloat = static_cast<float*>(CommandArray);
    
    for(int i=0; i<93; i++){
        CommandArrayFloat[i] = 0;
    }
}
