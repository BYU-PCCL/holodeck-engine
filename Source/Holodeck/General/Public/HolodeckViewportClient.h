// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "Engine/GameViewportClient.h"

#include "HolodeckViewportClient.generated.h"

/**
  * UHolodeckViewportClient
  * Handles capturing the main player camera image data.
  * Must be set in the editor to be the viewport client.
  *
  * THIS MUST BE IN DefaultEngine.ini
  * GameViewportClientClassName = /Script/Holodeck.HolodeckViewportClient
  */
UCLASS()
class HOLODECK_API UHolodeckViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor.
	  */
	UHolodeckViewportClient(const FObjectInitializer& PCIP);

	/**
	  * Draw
	  * Calls the parent draw function, and captures the pixels.
	  */
	void Draw(FViewport* Viewport, FCanvas* SceneCanvas) override;

	/**
	  * SetBuffer
	  * Sets the buffer to capture pixels to.
	  * @param NewBuffer the buffer to save the pixel data to.
	  */
	void SetBuffer(void* NewBuffer);

private:
	/**
	  * HolodeckTakeScreenShot
	  * Takes a screenshot of the main player camera.
	  * Must only be called from inside the draw function!
	  */
	void HolodeckTakeScreenShot();

	FColor* Buffer;
	FVector2D ViewportSize;
};
