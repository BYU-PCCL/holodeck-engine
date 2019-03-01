// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "AddSensorCommand.h"
#include "HolodeckSensor.h"

void UAddSensorCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("v::Add sensor"));

	if (StringParams.size() != 4 || NumberParams.size() != 0) {
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

	static USensorMapType SensorMap = { { "CollisionSensor", UCollisionSensor::StaticClass() },
										{ "IMUSensor", UIMUSensor::StaticClass() },
										{ "JointRotationSensor", UJointRotationSensor::StaticClass() },
										{ "LocationSensor", ULocationSensor::StaticClass() },
										{ "OrientationSensor", UOrientationSensor::StaticClass() },
										{ "PressureSensor", UPressureSensor::StaticClass() },
										{ "RelativeSkeletalPositionSensor", URelativeSkeletalPositionSensor::StaticClass() },
										{ "RGBCamera", URGBCamera::StaticClass() },
										{ "RotationSensor", URotationSensor::StaticClass() },
										{ "VelocitySensor", UVelocitySensor::StaticClass() },
										{ "ViewportCapture", UViewportCapture::StaticClass() },
										{ "DistanceTask", UDistanceTask::StaticClass() },
										{ "FollowTask", UFollowTask::StaticClass() } };

	FString AgentName = StringParams[0].c_str();
	FString SocketName = StringParams[1].c_str();
	FString SensorName = StringParams[2].c_str();
	FString TypeName = StringParams[3].c_str();
	AHolodeckAgent* Agent = GetAgent(AgentName);

	UHolodeckSensor* Sensor = NewObject<UHolodeckSensor>(Agent->GetRootComponent(), SensorMap[TypeName]);
	Sensor->SensorName = SensorName;

	if (Sensor && Agent)
	{
		Sensor->RegisterComponent();
		if (SocketName.IsEmpty()) {
			Sensor->AttachToComponent(Agent->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		}
		else {
			Sensor->AttachToComponent(Agent->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName(*SocketName));
		}

		Sensor->SetAgentAndController(Agent->HolodeckController, AgentName);
		Sensor->InitializeSensor();
		Agent->SensorMap.Add(Sensor->SensorName, Sensor);
	}
}
