// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckPawnController.h"
#include "ImageUtils.h"
#include "Misc/Base64.h"
#include "CameraSensorArray2D.h"


// Sets default values for this component's properties
UCameraSensorArray2D::UCameraSensorArray2D()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics; // Tick every physics substep?

}


// Called when the game starts
void UCameraSensorArray2D::BeginPlay()
{
	Super::BeginPlay();

	Controller = (AHolodeckPawnController*)(this->GetAttachmentRootActor()->GetInstigator()->Controller);

	//get all the attached USceneCaptureComponent2D
	for (USceneComponent* child : GetAttachChildren()) {
		USceneCaptureComponent2D* camera = Cast<USceneCaptureComponent2D>(child);

		if (camera)
			AAttachedCameras.Add(camera);
	}
}


// Called every frame
void UCameraSensorArray2D::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FHolodeckSensorData data = FHolodeckSensorData();
	data.Type = "CameraSensorArray2D";

	TMap < FString, FString> CaptureData;
	Capture(CaptureData);
	
	//Return base64 CaptureData as json object
	FString DataString = "[";
	for (auto& Item : CaptureData)
	{
		DataString += "{\"" + Item.Key + "\":\"" + Item.Value + "\"},";
	}
	DataString.RemoveAt(DataString.Len() - 1);
	DataString += "]";

	data.Data = DataString;

	Controller->Publish(data);
}

bool UCameraSensorArray2D::Capture(TMap<FString, FString>& output)
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting capture!"));
	for (USceneCaptureComponent2D* camera :AAttachedCameras) {
		if (camera->TextureTarget)
		{
			FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)camera->TextureTarget->Resource;
			TArray<FColor> ColorBuffer;
			
			if (textureResource && textureResource->ReadPixels(ColorBuffer)) {

				TArray<uint8> PNG_Compressed_ImageData;

				FImageUtils::CompressImageArray(
					textureResource->GetSizeX(),
					textureResource->GetSizeY(),
					ColorBuffer,
					PNG_Compressed_ImageData
				);

				FString base64data = FBase64::Encode(PNG_Compressed_ImageData);

				output.Add(camera->GetName(), base64data);

				//Save png file to a file to observe it for debugging
				//const TCHAR* PNGFileName = TEXT("C:\\path\\to\\file.png");
				//
				//FFileHelper::SaveArrayToFile(
				//	PNG_Compressed_ImageData,
				//	PNGFileName
				//);

				//empty buffers
				PNG_Compressed_ImageData.Empty();
				ColorBuffer.Empty();

				UE_LOG(LogTemp, Warning, TEXT("Successful capture with pixels!"));
			}
				
		}
	}
	
	return false;
}