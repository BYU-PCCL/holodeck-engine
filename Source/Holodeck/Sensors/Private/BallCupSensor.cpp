// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "BallCupSensor.h"
#include "HolodeckGameMode.h"

UBallCupSensor::UBallCupSensor() {
	PrimaryComponentTick.bCanEverTick = true;
	SensorName = "BallCupSensor";
}

void UBallCupSensor::InitializeSensor() {
	Super::InitializeSensor();
}

void UBallCupSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	//Check if the sensor is on and if it is retrieve the ball location from the world state
	if (bOn) {
		AActor* Target = GetWorld()->GetAuthGameMode();
		AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
		int* IntBuffer = static_cast<int*>(Buffer);
		IntBuffer[0] = (int32)Game->GetWorldNum("BallLocation");
	}
}
