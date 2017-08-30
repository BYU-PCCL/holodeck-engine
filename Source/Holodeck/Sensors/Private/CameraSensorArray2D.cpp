// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "ImageUtils.h"
#include "Misc/Base64.h"
#include "CameraSensorArray2D.h"


UCameraSensorArray2D::UCameraSensorArray2D() {
	PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.TickGroup = TG_PrePhysics; // Tick every physics substep?
}


// Called when the game starts
void UCameraSensorArray2D::BeginPlay() {
	Super::BeginPlay();
	/*
	//get all the attached USceneCaptureComponent2D
	for (USceneComponent* child : GetAttachChildren()) {
		USceneCaptureComponent2D* SceneCaptureComponent = Cast<USceneCaptureComponent2D>(child);

		if (SceneCaptureComponent)
			AAttachedSceneCaptureComponents2D.Add(SceneCaptureComponent);
	}
	*/
}


// Called every frame
void UCameraSensorArray2D::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	// if (Controller != nullptr)
	// 	Controller->Publish(data);
}

void UCameraSensorArray2D::Capture(TMap<FString, FString>& output) {
	/*
	// For each camera attached to this agent
	for (USceneCaptureComponent2D* camera :AAttachedSceneCaptureComponents2D) {
		if (camera->TextureTarget)
		{
			// Read the texture into ColorBuffer
			FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)camera->TextureTarget->Resource;
			TArray<FColor> ColorBuffer;
			bool bSuccessfulRead = textureResource->ReadPixels(ColorBuffer);
			
			if (textureResource && bSuccessfulRead) {

				if (ResultArray == nullptr)
					ResultArray = new char[ColorBuffer.Num() * HEX_COLOR_WIDTH * NUM_RGB_CHANNELS + 1]; // +1 for null char
				int Pos = 0;

				for (int i = 0; i < ColorBuffer.Num(); i++)
				{
					ResultArray[Pos] = RGBConv[ColorBuffer[i].B][0];
					ResultArray[Pos + 1] = RGBConv[ColorBuffer[i].B][1];
					Pos += 2;
					ResultArray[Pos] = RGBConv[ColorBuffer[i].G][0];
					ResultArray[Pos + 1] = RGBConv[ColorBuffer[i].G][1];
					Pos += 2;
					ResultArray[Pos] = RGBConv[ColorBuffer[i].R][0];
					ResultArray[Pos + 1] = RGBConv[ColorBuffer[i].R][1];
					Pos += 2;
				}
				ResultArray[Pos] = '\0';

				output.Add(camera->GetName(), ANSI_TO_TCHAR(ResultArray));
			}
		}
	}
	*/
}

FString UCameraSensorArray2D::GetDataKey() {
	return "CameraSensorArray2D";
}

int UCameraSensorArray2D::GetNumItems() {
	int numPixels = 0;
	int numCameras = AAttachedSceneCaptureComponents2D.Num();
	// For each camera attached to this agent
	for (USceneCaptureComponent2D* camera : AAttachedSceneCaptureComponents2D) {
		if (camera->TextureTarget)
		{
			// Read the texure into ColorBuffer
			FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)camera->TextureTarget->Resource;
			TArray<FColor> ColorBuffer;
			bool bSuccessfulRead = textureResource->ReadPixels(ColorBuffer);

			if (textureResource && bSuccessfulRead)
			{
				numPixels = ColorBuffer.Num();
				break;
			}
		}
	}
	return numCameras * numPixels;
}

int UCameraSensorArray2D::GetItemSize() {
	return sizeof(float);
}

//void UCameraSensorArray2D::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	/*TMap <FString, FString> CaptureData;
	Capture(CaptureData);

	//Return base64 CaptureData as json object
	FString DataString = "";
	for (auto& Item : CaptureData)
	{
		DataString += "{\"" + Item.Key + "\":\"" + Item.Value + "\"},";
	}

	if (DataString.Len() > 0)
		DataString.RemoveAt(DataString.Len() - 1);
	DataString.InsertAt(0, "[");
	DataString += "]";
	ResultData.Data = DataString;*/
//}

const char UCameraSensorArray2D::RGBConv[256][2] = { { '0','0' },{ '0','1' },{ '0','2' },{ '0','3' },{ '0','4' },{ '0','5' },{ '0','6' },{ '0','7' },{ '0','8' },{ '0','9' },{ '0','a' },{ '0','b' },{ '0','c' },{ '0','d' },{ '0','e' },{ '0','f' },
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
