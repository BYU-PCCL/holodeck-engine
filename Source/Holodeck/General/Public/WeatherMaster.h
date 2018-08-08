#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherMaster.generated.h"

/**
* WeatherMaster
* The actor which is used to alter the state of weather/sky/fog/etc in the world.
*
* The WeatherMaster should not be spawned or placed in world. It is programmatically 
* spawned in the HolodeckGameModeBP and saved for later use.
*
* The weather master is fully implemented in its derived blueprint so it can have access
* to other BPs in the world which cannot be accessed in C++. 
*/
UCLASS()
class HOLODECK_API AWeatherMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeatherMaster();

	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeFogDensity(const float& density);
	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeSunHeight(const float& angle);
	UFUNCTION(BlueprintImplementableEvent)
		bool MakeRain();
	UFUNCTION(BlueprintImplementableEvent)
		bool MakeCloudy();

	void StartDayCycle(const int32& DayLength);
	void StopDayCycle();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void SetDayCycleLength(const int32& DayLength);
};
