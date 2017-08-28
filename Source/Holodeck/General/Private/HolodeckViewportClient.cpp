// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "UAV.h"
#include "Benchmarker.h"
#include <stdlib.h>
#include "HolodeckViewportClient.h"

UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) {}

void UHolodeckViewportClient::HolodeckTakeScreenShot()
{
	// THIS MUST BE IN DefaultEngine.ini
	// GameViewportClientClassName = /Script/Holodeck.HolodeckViewportClient

	if (buffer != nullptr) {
		//Get the viewport size
		GetViewportSize(ViewportSize);
		if (ViewportSize.X <= 0 || ViewportSize.Y <= 0) return;
		bool bGotScreenshot = Viewport->ReadPixelsPtr(buffer, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(0, 0, ViewportSize.X, ViewportSize.Y));
	}
}

void UHolodeckViewportClient::Draw(FViewport * viewport, FCanvas * SceneCanvas)
{
	Super::Draw(viewport, SceneCanvas);
	HolodeckTakeScreenShot();
}

void UHolodeckViewportClient::SetBuffer(void* new_buffer) {
	this->buffer = static_cast<FColor*>(new_buffer);
}
