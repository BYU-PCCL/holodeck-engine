// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "UAV.h"
#include "Benchmarker.h"
#include <stdlib.h>
#include "HolodeckViewportClient.h"

UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) { 
	bHolodeckDoScreenShot = true; 
	bFirstTime = true; 
}

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
		if (!bGrayScale) {
			// Check whether result array has been initialized
			if (ResultArray == nullptr)
				ResultArray = new char[int(ViewportSize.X) * int(ViewportSize.Y) * HEX_COLOR_WIDTH * NUM_RGB_CHANNELS + 1]; // +1 for null char

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
		}
		else {
			if (ResultArray == nullptr)
				ResultArray = new char[int(ViewportSize.X) * int(ViewportSize.Y) * HEX_COLOR_WIDTH * NUM_GRAYSCALE_CHANNELS + 1]; // +1 for null char

			int Pos = 0;

			for (int i = 0; i < ViewportSize.X*ViewportSize.Y; i++)
			{
				uint8 blue = HolodeckColorBuffer[i].B;
				uint8 green = HolodeckColorBuffer[i].G;
				uint8 red = HolodeckColorBuffer[i].R;

				uint8 avg0 = RGBConv[(blue + green + red)/3][0];
				uint8 avg1 = RGBConv[(blue + green + red) / 3][1];

				ResultArray[Pos] = avg0;
				ResultArray[Pos + 1] = avg1;
				Pos += 2;
			}
			ResultArray[Pos] = '\0';
		}


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

const char UHolodeckViewportClient::RGBConv[256][2] = { { '0','0' },{ '0','1' },{ '0','2' },{ '0','3' },{ '0','4' },{ '0','5' },{ '0','6' },{ '0','7' },{ '0','8' },{ '0','9' },{ '0','a' },{ '0','b' },{ '0','c' },{ '0','d' },{ '0','e' },{ '0','f' },
{ '1','0' },{ '1','1' },{ '1','2' },{ '1','3' },{ '1','4' },{ '1','5' },{ '1','6' },{ '1','7' },{ '1','8' },{ '1','9' },{ '1','a' },{ '1','b' },{ '1','c' },{ '1','d' },{ '1','e' },{ '1','f' },
{ '2','0' },{ '2','1' },{ '2','2' },{ '2','3' },{ '2','4' },{ '2','5' },{ '2','6' },{ '2','7' },{ '2','8' },{ '2','9' },{ '2','a' },{ '2','b' },{ '2','c' },{ '2','d' },{ '2','e' },{ '2','f' },
{ '3','0' },{ '3','1' },{ '3','2' },{ '3','3' },{ '3','4' },{ '3','5' },{ '3','6' },{ '3','7' },{ '3','8' },{ '3','9' },{ '3','a' },{ '3','b' },{ '3','c' },{ '3','d' },{ '3','e' },{ '3','f' },
{ '4','0' },{ '4','1' },{ '4','2' },{ '4','3' },{ '4','4' },{ '4','5' },{ '4','6' },{ '4','7' },{ '4','8' },{ '4','9' },{ '4','a' },{ '4','b' },{ '4','c' },{ '4','d' },{ '4','e' },{ '4','f' },
{ '5','0' },{ '5','1' },{ '5','2' },{ '5','3' },{ '5','4' },{ '5','5' },{ '5','6' },{ '5','7' },{ '5','8' },{ '5','9' },{ '5','a' },{ '5','b' },{ '5','c' },{ '5','d' },{ '5','e' },{ '5','f' },
{ '6','0' },{ '6','1' },{ '6','2' },{ '6','3' },{ '6','4' },{ '6','5' },{ '6','6' },{ '6','7' },{ '6','8' },{ '6','9' },{ '6','a' },{ '6','b' },{ '6','c' },{ '6','d' },{ '6','e' },{ '6','f' },
{ '7','0' },{ '7','1' },{ '7','2' },{ '7','3' },{ '7','4' },{ '7','5' },{ '7','6' },{ '7','7' },{ '7','8' },{ '7','9' },{ '7','a' },{ '7','b' },{ '7','c' },{ '7','d' },{ '7','e' },{ '7','f' },
{ '8','0' },{ '8','1' },{ '8','2' },{ '8','3' },{ '8','4' },{ '8','5' },{ '8','6' },{ '8','7' },{ '8','8' },{ '8','9' },{ '8','a' },{ '8','b' },{ '8','c' },{ '8','d' },{ '8','e' },{ '8','f' },
{ '9','0' },{ '9','1' },{ '9','2' },{ '9','3' },{ '9','4' },{ '9','5' },{ '9','6' },{ '9','7' },{ '9','8' },{ '9','9' },{ '9','a' },{ '9','b' },{ '9','c' },{ '9','d' },{ '9','e' },{ '9','f' },
{ 'a','0' },{ 'a','1' },{ 'a','2' },{ 'a','3' },{ 'a','4' },{ 'a','5' },{ 'a','6' },{ 'a','7' },{ 'a','8' },{ 'a','9' },{ 'a','a' },{ 'a','b' },{ 'a','c' },{ 'a','d' },{ 'a','e' },{ 'a','f' },
{ 'b','0' },{ 'b','1' },{ 'b','2' },{ 'b','3' },{ 'b','4' },{ 'b','5' },{ 'b','6' },{ 'b','7' },{ 'b','8' },{ 'b','9' },{ 'b','a' },{ 'b','b' },{ 'b','c' },{ 'b','d' },{ 'b','e' },{ 'b','f' },
{ 'c','0' },{ 'c','1' },{ 'c','2' },{ 'c','3' },{ 'c','4' },{ 'c','5' },{ 'c','6' },{ 'c','7' },{ 'c','8' },{ 'c','9' },{ 'c','a' },{ 'c','b' },{ 'c','c' },{ 'c','d' },{ 'c','e' },{ 'c','f' },
{ 'd','0' },{ 'd','1' },{ 'd','2' },{ 'd','3' },{ 'd','4' },{ 'd','5' },{ 'd','6' },{ 'd','7' },{ 'd','8' },{ 'd','9' },{ 'd','a' },{ 'd','b' },{ 'd','c' },{ 'd','d' },{ 'd','e' },{ 'd','f' },
{ 'e','0' },{ 'e','1' },{ 'e','2' },{ 'e','3' },{ 'e','4' },{ 'e','5' },{ 'e','6' },{ 'e','7' },{ 'e','8' },{ 'e','9' },{ 'e','a' },{ 'e','b' },{ 'e','c' },{ 'e','d' },{ 'e','e' },{ 'e','f' },
{ 'f','0' },{ 'f','1' },{ 'f','2' },{ 'f','3' },{ 'f','4' },{ 'f','5' },{ 'f','6' },{ 'f','7' },{ 'f','8' },{ 'f','9' },{ 'f','a' },{ 'f','b' },{ 'f','c' },{ 'f','d' },{ 'f','e' },{ 'f','f' } };