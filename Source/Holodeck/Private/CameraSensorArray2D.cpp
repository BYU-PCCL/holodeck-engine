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

	AController* tmp = this->GetAttachmentRootActor()->GetInstigator()->Controller;
	Controller = Cast<AHolodeckPawnController>(this->GetAttachmentRootActor()->GetInstigator()->Controller);

	UE_LOG(LogTemp, Warning, TEXT("%s"), tmp);

	if (Controller == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid Controller: Agent is not being controlled by child of HolodeckPawnController"));
	}


	// TODO: should be moved to a composable sensor class instaed
	MessageEndpoint = FMessageEndpoint::Builder("FHolodeckPawnControllerMessageEndpoint");

}


// Called every frame
void UCameraSensorArray2D::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


	//if (Controller) {
		FHolodeckSensorData data = FHolodeckSensorData();
		data.Type = "CameraSensorArray2D";

		TMap < FString, FString> CaptureData;
		Capture(CaptureData);
	
		//Return base64 CaptureData as json object
		FString DataString = "{";
		for (auto& Item : CaptureData)
		{
			DataString += "\"" + Item.Key + "\":\"" + Item.Value + "\",";
		}
		DataString.RemoveAt(DataString.Len() - 1);
		DataString += "}";

		data.Data = DataString;

		FHolodeckResponse* response = new FHolodeckResponse();
		response->Source = TEXT("UAV"); // this->GetAttachmentRootActor()->GetHumanReadableName(); //get tag 0 if it's there and use that instead of GetHumanReadableName()... otherwise just use GetHumanReadableName();

		response->Type = data.Type;
		response->Data = data.Data;

		MessageEndpoint->Publish<FHolodeckResponse>(response);

		//Controller->Publish(data);
	//}
}

bool UCameraSensorArray2D::Capture(TMap<FString, FString>& output)
{
	for (USceneComponent* child : GetAttachChildren()) {
		USceneCaptureComponent2D* camera = Cast<USceneCaptureComponent2D>(child);
		if (camera && camera->TextureTarget)
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

				output.Add(child->GetName(), base64data);

				// TODO: Investigate if this is nessesary: empty the buffers
				PNG_Compressed_ImageData.Empty();
				ColorBuffer.Empty();

			}
				
		}
	}
	
	return false;
}