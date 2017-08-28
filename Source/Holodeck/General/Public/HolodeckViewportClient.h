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
	void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;
	void SetBuffer(void* buffer);

private:
	//Only run from draw()!
	void HolodeckTakeScreenShot();
	FColor* buffer;
	FVector2D ViewportSize;
};