// Fill out your copyright notice in the Description page of Project Settings.

#include "HolodeckPrivatePCH.h"
#include "HolodeckWorldSettings.h"

float AHolodeckWorldSettings::FixupDeltaSeconds(float DeltaSeconds, float RealDeltaSeconds) {
	return ConstantTimeDeltaBetweenTicks;
}

float AHolodeckWorldSettings::GetConstantTimeDeltaBetweenTicks() {
	return ConstantTimeDeltaBetweenTicks;
}

void AHolodeckWorldSettings::SetConstantTimeDeltaBetweenTicks(float delta) {
	ConstantTimeDeltaBetweenTicks = delta;
}