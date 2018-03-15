#include "Holodeck.h"
#include "SpawnUAVCommand.h"

USpawnUAVCommand::USpawnUAVCommand(FVector Location) {
	this->Location = Location;

}
USpawnUAVCommand::USpawnUAVCommand() {
	this->Location = FVector(0, 0, 0); //default location to spawn. 
}

void USpawnUAVCommand::Execute(){
	//GetWorld()->SpawnActor(AKAsset::StaticClass(), NAME_None, &Location);
}

FString USpawnUAVCommand::GetNewName() {
	static int spawnedCount = 0;
	return "This is not yet implemented";
}