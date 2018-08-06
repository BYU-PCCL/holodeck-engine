#include "Holodeck.h"
#include "WeatherMaster.h"


// Sets default values
AWeatherMaster::AWeatherMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeatherMaster::BeginPlay()
{
	Super::BeginPlay();
	
}
