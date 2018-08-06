#include "Holodeck.h"
#include "WeatherMaster.h"


// Sets default values
AWeatherMaster::AWeatherMaster()
{
 	// Should not tick every frame
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeatherMaster::BeginPlay()
{
	Super::BeginPlay();
	
}
