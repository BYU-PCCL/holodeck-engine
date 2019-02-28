// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "AddSensorCommand.h"
#include "CollisionSensor.h"
#include "HolodeckCamera.h"
#include "IMUSensor.h"
#include "JointRotationSensor.h"
#include "LocationSensor.h"
#include "OrientationSensor.h"
#include "PressureSensor.h"
#include "RelativeSkeletalPositionSensor.h"
#include "RGBCamera.h"
#include "RotationSensor.h"
#include "VelocitySensor.h"
#include "ViewportCapture.h"
#include "DistanceTask.h"
#include "FollowTask.h"

void UAddSensorCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("v::Add sensor"));

	if (StringParams.size() != 2 || NumberParams.size() != 1) {
		UE_LOG(LogHolodeck, Error, TEXT("Unexpected argument length found in v. Command not executed."));
		return;
	}

	AHolodeckGameMode* GameTarget = static_cast<AHolodeckGameMode*>(Target);
	if (GameTarget == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UCommand::Target is not a UHolodeckGameMode*. UAddSensorCommand::Sensor not added."));
		return;
	}

	UWorld* World = Target->GetWorld();
	if (World == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("UAddSensorCommand::Execute found world as nullptr. Sensor not added."));
		return;
	}

	FString AgentName = StringParams[0].c_str();
	FString SocketName = StringParams[1].c_str();
	FString SensorName = StringParams[2].c_str();
	FString TypeName = StringParams[3].c_str();
	AHolodeckAgent* Agent = GetAgent(AgentName);

	UHolodeckSensor* Sensor = nullptr;
	if (TypeName == "CollisionSensor") {
		Sensor = NewObject<UCollisionSensor>();
	}
	else if (TypeName == "HolodeckCamera") {
		Sensor = NewObject<UHolodeckCamera>();
	}
	else if (TypeName == "IMUSensor") {
		Sensor = NewObject<UIMUSensor>();
	}
	else if (TypeName == "JointRotationSensor") {
		Sensor = NewObject<UJointRotationSensor>();
	}
	else if (TypeName == "LocationSensor") {
		Sensor = NewObject<ULocationSensor>();
	}
	else if (TypeName == "OrientationSensor") {
		Sensor = NewObject<UOrientationSensor>();
	}
	else if (TypeName == "PressureSensor") {
		Sensor = NewObject<UPressureSensor>();
	}
	else if (TypeName == "RelativeSkeletalPositionSensor") {
		Sensor = NewObject<URelativeSkeletalPositionSensor>();
	}
	else if (TypeName == "RGBCamera") {
		Sensor = NewObject<URGBCamera>();
	}
	else if (TypeName == "RotationSensor") {
		Sensor = NewObject<URotationSensor>();
	}
	else if (TypeName == "VelocitySensor") {
		Sensor = NewObject<UVelocitySensor>();
	}
	else if (TypeName == "ViewportCapture") {
		Sensor = NewObject<UViewportCapture>();
	}
	else if (TypeName == "DistanceTask") {
		Sensor = NewObject<UDistanceTask>();
	}
	else if (TypeName == "FollowTask") {
		Sensor = NewObject<UFollowTask>();
	}

	if (Sensor && Agent)
	{
		Sensor->RegisterComponent();
		if (SocketName.IsEmpty()) {
			Sensor->AttachToComponent(Cast<USceneComponent>(Agent), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		}
		else {
			Sensor->AttachToComponent(Cast<USceneComponent>(Agent), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName(*SocketName));
		}

		Sensor->SetAgentAndController(Agent->HolodeckController, AgentName);
		Sensor->InitializeSensor();
		Agent->SensorMap.Add(Sensor->SensorName, Sensor);
	}
}
