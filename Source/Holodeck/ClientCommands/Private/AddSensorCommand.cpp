// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "AddSensorCommand.h"
#include "HolodeckSensor.h"

void UAddSensorCommand::Execute() {
	UE_LOG(LogHolodeck, Log, TEXT("UAddSensorCommand::Add sensor"));

	if (StringParams.size() != 5 || NumberParams.size() != 6) {
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
										{ "LocationTask", ULocationTask::StaticClass() },
										{ "FollowTask", UFollowTask::StaticClass() } };

	FString AgentName = StringParams[0].c_str();
	FString SensorName = StringParams[1].c_str();
	FString TypeName = StringParams[2].c_str();
	FString ParmsJson = StringParams[3].c_str();
	FString SocketName = StringParams[4].c_str();
	int LocationX = NumberParams[0];
	int LocationY = NumberParams[1];
	int LocationZ = NumberParams[2];
	int RotationPitch = NumberParams[3];
	int RotationYaw = NumberParams[4];
	int RotationRoll = NumberParams[5];

	AHolodeckAgent* Agent = GetAgent(AgentName);

	UHolodeckSensor* Sensor = NewObject<UHolodeckSensor>(Agent->GetRootComponent(), SensorMap[TypeName]);
	Sensor->SensorName = SensorName;
	Sensor->ParseSensorParms(ParmsJson);
	Sensor->SetRelativeLocation(ConvertLinearVector(FVector(LocationX, LocationY, LocationZ), ClientToUE));
	Sensor->SetRelativeRotation(FRotator(RotationPitch, RotationYaw, RotationRoll));

	if (Sensor && Agent)
	{
		Sensor->RegisterComponent();

		if (SocketName.IsEmpty()) {
			Sensor->AttachToComponent(Agent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		}
		else {
			Sensor->AttachToComponent(Agent->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, FName(*SocketName));
		}

		Sensor->SetAgentAndController(Agent->HolodeckController, AgentName);
		Sensor->InitializeSensor();
		Agent->SensorMap.Add(Sensor->SensorName, Sensor);
	}
}
