// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "UAV.h"
#include "Benchmarker.h"
#include <stdlib.h>
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
		
		//TArray<uint8> PNG_Compressed_ImageData;
		//this->CompressImageArrayJPEG(
		//	ViewportSize.X,
		//	ViewportSize.Y,
		//	HolodeckColorBuffer,
		//	PNG_Compressed_ImageData
		//);
		
		static char* ResultArray = new char[int(ViewportSize.X) * int(ViewportSize.Y) * 2 * 3 + 1]; // 2 for 2 hexadecimal digits, 3 for rgb channels, +1 for null char
		int Pos = 0;

		for (int i = 0; i < ViewportSize.X*ViewportSize.Y; i++)
		{
			ResultArray[Pos] = RGBConv[HolodeckColorBuffer[i].B][0];
			ResultArray[Pos + 1] = RGBConv[HolodeckColorBuffer[i].B][1];
			Pos += 2;
			ResultArray[Pos] = RGBConv[HolodeckColorBuffer[i].G][0];
			ResultArray[Pos + 1] = RGBConv[HolodeckColorBuffer[i].G][1];
			Pos += 2;
			ResultArray[Pos] = RGBConv[HolodeckColorBuffer[i].R][0];
			ResultArray[Pos + 1] = RGBConv[HolodeckColorBuffer[i].R][1];
			Pos += 2;
		}
		ResultArray[Pos] = '\0';
		if (ImageQueue.IsEmpty())
		{
			FString FinalResult(ANSI_TO_TCHAR(ResultArray));
			ImageQueue.Enqueue(FinalResult);
		}
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

void UHolodeckViewportClient::CompressImageArrayJPEG(int32 ImageWidth, int32 ImageHeight, TArray<FColor> &SrcData, TArray<uint8> &DstData)
{
	// PNGs are saved as RGBA but FColors are stored as BGRA. An option to swap the order upon compression may be added at some point. At the moment, manually swapping Red and Blue 
	for (int32 Index = 0; Index < ImageWidth*ImageHeight; Index++)
	{
		uint8 TempRed = SrcData[Index].R;
		SrcData[Index].R = SrcData[Index].B;
		SrcData[Index].B = TempRed;
	}
	FObjectThumbnail TempThumbnail;
	TempThumbnail.SetImageSize(ImageWidth, ImageHeight);
	TArray<uint8>& ThumbnailByteArray = TempThumbnail.AccessImageData();

	// Copy scaled image into destination thumb
	int32 MemorySize = ImageWidth*ImageHeight * sizeof(FColor);
	ThumbnailByteArray.AddUninitialized(MemorySize);
	FMemory::Memcpy(ThumbnailByteArray.GetData(), SrcData.GetData(), MemorySize);

	// Compress data - convert into a .png
	TempThumbnail.CompressImageData();
	TArray<uint8>& CompressedByteArray = TempThumbnail.AccessCompressedImageData();
	DstData = TempThumbnail.AccessCompressedImageData();
}