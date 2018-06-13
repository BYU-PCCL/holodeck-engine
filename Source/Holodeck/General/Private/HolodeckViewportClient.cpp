// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClient.h"
#include "HolodeckCamera.h" //Included here to avoid cyclic dependency. 

UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) {}

void UHolodeckViewportClient::HolodeckTakeScreenShot() {
	if (Buffer != nullptr) {
		GetViewportSize(ViewportSize);
		if (ViewportSize.X <= 0 || ViewportSize.Y <= 0) return;
		bool bGotScreenshot = Viewport->ReadPixelsPtr(Buffer, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(0, 0, ViewportSize.X, ViewportSize.Y));
	}
}

void UHolodeckViewportClient::Draw(FViewport * ViewportParam, FCanvas * SceneCanvas) {
	Super::Draw(ViewportParam, SceneCanvas);
	HolodeckTakeScreenShot();
	UE_LOG(LogHolodeck, Log, TEXT("@@@@@ Draw called"));
	for (auto& Cam : Cameras) {
		Cam.Value->Capture();
	}

	UE_LOG(LogHolodeck, Log, TEXT("@@@@@ Draw Ended"));
}

void UHolodeckViewportClient::SetBuffer(void* NewBuffer) {
	this->Buffer = static_cast<FColor*>(NewBuffer);
}

void UHolodeckViewportClient::AddCamera(UHolodeckCamera* Camera) {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckViewportClient::AddCamera called"));
	this->Cameras.Add(Camera->GetAgentName(), Camera);
}
