// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClient.h"


UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) { bHolodeckDoScreenShot = true; bFirstTime = true; }

void UHolodeckViewportClient::HolodeckTakeScreenShot()
{
	// THIS MUST BE IN DefaultEngine.ini
	// GameViewportClientClassName = /Script/Holodeck.HolodeckViewportClient

	//Get the viewport size
	FVector2D viewportSize;
	GetViewportSize(viewportSize);

	//output viewport size, useful for debugging
	/*
	FString text = "x= ";
	text += FString::FromInt(viewportSize.X);
	text += "y= ";
	text += FString::FromInt(viewportSize.Y);
	GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, text);
	*/
	
	//check the viewport size is valid
	if (viewportSize.X <= 0 || viewportSize.Y <= 0) return;

	// First time setup
	if (bFirstTime)
	{
		//Empty color buffer?
		HolodeckColorBuffer.Empty();

		//Zero the color buffer
		HolodeckColorBuffer.AddZeroed(viewportSize.X * viewportSize.Y);
		bFirstTime = false;
	}

	static Benchmarker b;
	static int32 count = 0;
	b.Start();
	bool bGotScreenshot = Viewport->ReadPixels(HolodeckColorBuffer, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(0, 0, viewportSize.X, viewportSize.Y));
	b.End();
	b.CalculateAvg();

	if (count++ == 30)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, b.Stat());
		FString result = TEXT("Read successful! 256000: ") + HolodeckColorBuffer[256000].ToString() + TEXT("; 14000: ") + HolodeckColorBuffer[14000].ToString() + TEXT("; 21000: ") + HolodeckColorBuffer[21000].ToString();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, result);
		count = 0;
	}

	//Get Screen Shot!
	if (!bGotScreenshot) 
	{
		//This was here: VictoryPC->Optimize("Failed to ReadPixels");
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Black, TEXT("Couldn't read pixels!"));
		return;
	}

	// If the screenshot was successful
	/*if (bScreenshotSuccessful && !FirstTime)
	{
	TArray<uint8> PNG_Compressed_ImageData;
	FImageUtils::CompressImageArray(
	SizeX,
	SizeY,
	Bitmap,
	PNG_Compressed_ImageData
	);

	//FString result = TEXT("7000: ") + Bitmap[7000].ToString() + TEXT("; 14000: ") + Bitmap[14000].ToString() + TEXT("; 21000: ") + Bitmap[21000].ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Red, result);

	//Save png file to a file to observe it for debugging
	//const TCHAR* PNGFileName = TEXT("C:\Users\\robert.pottorff\\Desktop\\output.png");
	//
	//FFileHelper::SaveArrayToFile(
	//	PNG_Compressed_ImageData,
	//	PNGFileName
	//);

	FString base64data = FBase64::Encode(PNG_Compressed_ImageData);

	// Add to the output
	output.Add(FString(TEXT("Main Camera")), base64data);
	}
	*/
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
	/*
	//Take Screen shot?
	if (bHolodeckDoScreenShot)
	{
		//bHolodeckDoScreenShot = false;
		HolodeckTakeScreenShot();
	}
	*/
}