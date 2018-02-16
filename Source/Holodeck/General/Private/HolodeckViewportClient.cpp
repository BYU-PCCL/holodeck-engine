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
	int i = 0;
	while (i < Cameras.size()) {
		if (Cameras[i] == nullptr) {
			Cameras.erase(Cameras.begin() + i);
		} else {
			Cameras[i]->Capture();
			i++;
		}
	}

	UE_LOG(LogHolodeck, Log, TEXT("@@@@@ Draw Ended"));
}

void UHolodeckViewportClient::SetBuffer(void* NewBuffer) {
	this->Buffer = static_cast<FColor*>(NewBuffer);
}

void UHolodeckViewportClient::AddCamera(UHolodeckCamera* Camera) {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckViewportClient::AddCamera called"));
	this->Cameras.push_back(Camera);
}

//void UHolodeckViewportClient::ClearNullCameras() {
//	int i = 0;
//	while( i < Cameras.Num()){
//		if (Cameras[i] == nullptr) {
//			Cameras.RemoveAt(i);
//		}
//		else {
//			i++;
//		}
//	}
//}
