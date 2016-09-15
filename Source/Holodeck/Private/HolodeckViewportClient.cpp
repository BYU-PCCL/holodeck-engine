// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckViewportClient.h"


UHolodeckViewportClient::UHolodeckViewportClient(const class FObjectInitializer& PCIP) : Super(PCIP) { bHolodeckDoScreenShot = true; bFirstTime = true; }

void UHolodeckViewportClient::HolodeckTakeScreenShot()
{
	// THIS MUST BE IN EdfaultEngine.ini
	// GameViewportClientClassName = / Script / Holodeck.HolodeckViewportClient

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

	viewportSize.X--;
	viewportSize.Y--;

	if (bFirstTime)
	{
		//Empty color buffer?
		HolodeckColorBuffer.Empty();

		//Zero the color buffer
		HolodeckColorBuffer.AddZeroed(viewportSize.X * viewportSize.Y);
		bFirstTime = false;
	}

	if (!Viewport->ReadFloat16Pixels(HolodeckFloat16ColorBuffer))
	//Get Screen Shot!
	//if (!Viewport->ReadPixels(
	//	HolodeckColorBuffer,
	//	FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX),
	//	FIntRect(1, 1, viewportSize.X, viewportSize.Y)
	//	)) 
	{
		//This was here: VictoryPC->Optimize("Failed to ReadPixels");
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Black, TEXT("Couldn't read pixels!"));
		return;
	}
	else
	{
		static EasyFileManager m;
		FString result = TEXT("");


		for (FFloat16Color& c : HolodeckFloat16ColorBuffer)
		{
			result += FString::SanitizeFloat(c.R) + FString::SanitizeFloat(c.G) + FString::SanitizeFloat(c.B);
		}

		//for (FColor& c : HolodeckColorBuffer)
		//{
		//	result += FString::FromInt(c.ToPackedRGBA());
		//	//result += c.ToString() + ", ";
		//}


		m.SaveToFile(result, "test.txt");
		//result = TEXT("Read successful! 1,024,000: ") + HolodeckColorBuffer[256000].ToString() + TEXT("; 14000: ") + HolodeckColorBuffer[14000].ToString() + TEXT("; 21000: ") + HolodeckColorBuffer[21000].ToString();
		GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Black, result);
	}
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