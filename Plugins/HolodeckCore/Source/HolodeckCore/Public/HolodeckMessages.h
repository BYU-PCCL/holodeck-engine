#pragma once

#include <vector>
#include "Json.h"
#include "HolodeckMessages.generated.h"

USTRUCT()
struct HOLODECKCORE_API FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	FHolodeckCommand() { }
};

USTRUCT()
struct HOLODECKCORE_API FHolodeckAndroidCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	TArray<float> ConstraintVector;

	FHolodeckAndroidCommand() : Target(), ConstraintVector() { }
};

USTRUCT()
struct HOLODECKCORE_API FHolodeckAndroidConfiguration : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	bool AreCollisionsVisible;

	FHolodeckAndroidConfiguration() : Target(), AreCollisionsVisible(false) { }
};

USTRUCT()
struct HOLODECKCORE_API FHolodeckUAVCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	float LocalRoll;
	
	UPROPERTY()
	float LocalPitch;
	
	UPROPERTY()
	float LocalYaw;

	FHolodeckUAVCommand() :
		Target(),
		LocalRoll(0),
		LocalPitch(0), 
		LocalYaw(0) { }
};

USTRUCT()
struct HOLODECKCORE_API FHolodeckUAVConfiguration : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	// TODO: create a similar New message HolodeckSensorConfiguration
	// FString Sensor
	// bool IsActive

	FHolodeckUAVConfiguration() : Target() { }
};

USTRUCT()
struct HOLODECKCORE_API FHolodeckSimulatorCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	uint8 AllowedTicksBetweenCommands;

	UPROPERTY()
	float TimeDeltaBetweenTicks;

	UPROPERTY()
	bool Restart;

	UPROPERTY()
	FString LoadLevel;

	FHolodeckSimulatorCommand() :  
		Target(),
		AllowedTicksBetweenCommands(0),
		TimeDeltaBetweenTicks(0),
		Restart(0),
		LoadLevel() { }
};


USTRUCT()
struct HOLODECKCORE_API FHolodeckCommandMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Action;

	UPROPERTY()
	FString Target;

	UPROPERTY()
	FString CommandType;

	UPROPERTY()
	FString CommandJSON;

	FHolodeckCommandMessage() : 
		Action(),
		Target(),
		CommandType(),
		CommandJSON() { }
};



USTRUCT()
struct HOLODECKCORE_API FHolodeckResponse
{
	GENERATED_USTRUCT_BODY()
		
	UPROPERTY()
	FString Source;
	
	UPROPERTY()
	FString Type;
	
	UPROPERTY()
	FString Data;

	FHolodeckResponse() :
		Source(),
		Type(),
		Data(){ }
};


USTRUCT()
struct HOLODECKCORE_API FHolodeckSensorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Type;

	UPROPERTY()
	FString Data;

	FHolodeckSensorData() :
		Type(),
		Data() { }
};