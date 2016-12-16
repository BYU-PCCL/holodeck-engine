// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "UAV.h"
#include "HolodeckViewportClient.h"


UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) { bHolodeckDoScreenShot = true; bFirstTime = true; }

void UHolodeckViewportClient::HolodeckTakeScreenShot()
{
	// THIS MUST BE IN DefaultEngine.ini
	// GameViewportClientClassName = /Script/Holodeck.HolodeckViewportClient
	//Get the viewport size
	GetViewportSize(ViewportSize);
	
	//check the viewport size is valid
	if (ViewportSize.X <= 0 || ViewportSize.Y <= 0) return;

	// First time setup
	if (bFirstTime)
	{
		//Empty color buffer?
		HolodeckColorBuffer.Empty();

		//Zero the color buffer
		HolodeckColorBuffer.AddZeroed(ViewportSize.X * ViewportSize.Y);
		bFirstTime = false;
	}

	bool bGotScreenshot = Viewport->ReadPixels(HolodeckColorBuffer, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(0, 0, ViewportSize.X, ViewportSize.Y));

	//Get Screen Shot!
	if (!bGotScreenshot) 
	{
		//This was here: VictoryPC->Optimize("Failed to ReadPixels");
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Black, TEXT("Couldn't read pixels!"));
		return;
	}
	else {
		TArray<uint8> PNG_Compressed_ImageData;
		FImageUtils::CompressImageArray(
			ViewportSize.X,
			ViewportSize.Y,
			HolodeckColorBuffer,
			PNG_Compressed_ImageData
		);

		FString result;
		for (uint8& num : PNG_Compressed_ImageData)
			result += FString::FromInt(num) + ",";
		//Base64Data = FBase64::Encode(PNG_Compressed_ImageData);
		//ImageQueue.Enqueue(Base64Data);
		ImageQueue.Enqueue(result);
	}

	return;
}


//Draw
void UHolodeckViewportClient::Draw(FViewport * Viewport, FCanvas * SceneCanvas)
{
	//GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, TEXT("Using HolodeckViewportClient!"));
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//this line is reaaaaaaaaaaaaaalllly important
	Super::Draw(Viewport, SceneCanvas);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//UE_LOG(Victory,Warning,TEXT("VICTORY GAME VIEWPORT Ticking!"));
	HolodeckTakeScreenShot();
}