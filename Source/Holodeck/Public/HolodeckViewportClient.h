// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Benchmarker.h"
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
	TArray<FColor> HolodeckColorBuffer;
private:
	//Only run from draw()!
	void HolodeckTakeScreenShot();
	bool bFirstTime;

	
	

};
