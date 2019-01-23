// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckTask.h"


// Sets default values
UHolodeckTask::UHolodeckTask() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHolodeckTask::InitializeSensor() {
	Super::InitializeSensor();
	//You need to get the pointer to the object you are attached to. 
	Parent = this->GetAttachParent();
}

// Called every frame
void UHolodeckTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (Parent != nullptr && bOn) {
		float* FloatBuffer = static_cast<float*>(Buffer);
		FloatBuffer[0] = Reward;
		FloatBuffer[1] = Terminal;
	}
}
