#pragma once

#include "HolodeckMessages.generated.h"

USTRUCT()
struct FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	FHolodeckCommand() { }
};

USTRUCT()
struct FHolodeckAndroidCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	TArray<float> ConstraintVector;

	FHolodeckAndroidCommand() : Target(), ConstraintVector() { }
};

USTRUCT()
struct FHolodeckAndroidConfiguration : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	UPROPERTY()
	bool AreCollisionsVisible;

	FHolodeckAndroidConfiguration() : Target(), AreCollisionsVisible(false) { }
};

USTRUCT()
struct FHolodeckUAVCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()
		UPROPERTY()
		FString Target;
	UPROPERTY()
		float Roll;

	UPROPERTY()
		float Pitch;

	UPROPERTY()
		float YawRate;
	UPROPERTY()
		float Altitude;
	FHolodeckUAVCommand() :
		Target(),
		Roll(0),
		Pitch(0),
		YawRate(0),
		Altitude(0) { }
};

USTRUCT()
struct FHolodeckUAVConfiguration : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Target;

	// TODO: create a similar New message HolodeckSensorConfiguration
	// FString Sensor
	// bool IsActive

	FHolodeckUAVConfiguration() : Target() { }
};

USTRUCT()
struct FHolodeckSphereRobotCommand : public FHolodeckCommand {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString Target;

	UPROPERTY()
		float Forward;

	UPROPERTY()
		float Right;

	FHolodeckSphereRobotCommand() :
		Target(),
		Forward(0),
		Right(0) { }
};

USTRUCT()
struct FHolodeckSimulatorCommand : public FHolodeckCommand {
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
struct FHolodeckCommandMessage
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
struct FHolodeckSensorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString AgentName;

	UPROPERTY()
	FString Key;

	UPROPERTY()
	FString Data;

	FHolodeckSensorData() :
		AgentName(),
		Key(),
		Data() { }
};