#include "Holodeck.h"
#include "WeatherMaster.h"


// Sets default values
AWeatherMaster::AWeatherMaster()
{
 	// Should not tick every frame unless the day cycle is turned on
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeatherMaster::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AWeatherMaster::StartDayCycle(const int32& DayLength)
{
	//enable tick and tell the blueprint to alter the day length
	SetActorTickEnabled(true);
	SetDayCycleLength(DayLength);
}

void AWeatherMaster::StopDayCycle()
{
	SetActorTickEnabled(false);
}
