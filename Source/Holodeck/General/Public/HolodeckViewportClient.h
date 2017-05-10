// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Benchmarker.h"
#include "ImageUtils.h"
#include "Misc/Base64.h"
#include "EasyFileManager.h"
#include "Engine/GameViewportClient.h"
#include "HolodeckViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API UHolodeckViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	UHolodeckViewportClient(const FObjectInitializer& PCIP);
	bool bHolodeckDoScreenShot;
	virtual void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;
	void CompressImageArrayJPEG(int32 ImageWidth, int32 ImageHeight, TArray<FColor>& SrcData, TArray<uint8>& DstData);
	TArray<FColor> HolodeckColorBuffer;
	FVector2D ViewportSize;
	FString Base64Data;

	TQueue<FString> ImageQueue;

	bool bGrayScale;

private:
	//Only run from draw()!
	void HolodeckTakeScreenShot();
	bool bFirstTime;

	int Red;
	int Green;
	int Blue;

	char* ResultArray;

	const static int NUM_RGB_CHANNELS = 3;
	const static int NUM_GRAYSCALE_CHANNELS = 1;
	const static int HEX_COLOR_WIDTH = 2;

	const static char RGBConv[256][2];
};