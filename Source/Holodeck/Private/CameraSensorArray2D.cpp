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
		USceneCaptureComponent2D* SceneCaptureComponent = Cast<USceneCaptureComponent2D>(child);
		UCameraComponent* Camera = Cast<UCameraComponent>(child);

		if (SceneCaptureComponent)
			AAttachedSceneCaptureComponents2D.Add(SceneCaptureComponent);
		else if (Camera)
			AAttachedCameras.Add(Camera);
	}

	FString displayAttachedComponents = FString(TEXT("SceneCaptureComponents: "));
	int32 count = 0;
	for (auto c : AAttachedSceneCaptureComponents2D)
		count++;
	displayAttachedComponents += FString::FromInt(count);
	displayAttachedComponents += TEXT(", Cameras: ");
	count = 0;
	for (auto c : AAttachedCameras)
		count++;
	displayAttachedComponents += FString::FromInt(count);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, displayAttachedComponents);
}


// Called every frame
void UCameraSensorArray2D::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHolodeckSensorData data = FHolodeckSensorData();
	data.Type = "CameraSensorArray2D";

	TMap < FString, FString> CaptureData;
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
	data.Data = DataString;

	Controller->Publish(data);
}

bool UCameraSensorArray2D::Capture(TMap<FString, FString>& output)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting capture!"));
	for (USceneCaptureComponent2D* camera :AAttachedSceneCaptureComponents2D) {
		if (camera->TextureTarget)
		{
			FTextureRenderTarget2DResource* textureResource = (FTextureRenderTarget2DResource*)camera->TextureTarget->Resource;
			TArray<FColor> ColorBuffer;
			
			if (textureResource && textureResource->ReadPixels(ColorBuffer)) {

				TArray<uint8> PNG_Compressed_ImageData;

				FIntPoint size = textureResource->GetSizeXY();

				FImageUtils::CompressImageArray(
					size.X,
					size.Y,
					ColorBuffer,
					PNG_Compressed_ImageData
				);

				FString base64data = FBase64::Encode(PNG_Compressed_ImageData);

				output.Add(camera->GetName(), base64data);

				// TODO: Investigate if this is nessesary: empty the buffers
				PNG_Compressed_ImageData.Empty();
				ColorBuffer.Empty();

			}
				
		}
	}
	
	return false;
}