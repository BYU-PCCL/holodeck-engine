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
	static bool FirstTime = true;
	//FlushRenderingCommands();

	// Check the engine and viewports are valid
	if (GEngine == NULL)
		return false;
	if (GEngine->GameViewport == NULL)
		return false;
	if (GEngine->GameViewport->Viewport == NULL)
		return false;

	// Get a pointer to the viewport
	FViewport* Viewport = GEngine->GameViewport->Viewport;
	TSharedPtr<SWindow> WindowPtr = GEngine->GameViewport->GetWindow();

	// Keep track of whether a screenshot was taken
	bool bScreenshotSuccessful = false;

	// Get the viewport size
	int32 SizeX = Viewport->GetSizeXY().X;
	int32 SizeY = Viewport->GetSizeXY().Y;

	// Prepare the screenshot holder
	TArray<FColor> Bitmap;

	// Get the screenshot
	if (WindowPtr.IsValid() && FSlateApplication::IsInitialized() && FirstTime == false)
	{
		FIntVector Size(SizeX, SizeY, 0);
		TSharedRef<SWidget> WindowRef = WindowPtr.ToSharedRef();

		static Benchmarker b;
		static int32 count = 0;
		b.Start();
		bScreenshotSuccessful = FSlateApplication::Get().TakeScreenshot(WindowRef, Bitmap, Size);

		b.End();
		b.CalculateAvg();
		if (count++ == 10)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, b.Stat());
			count = 0;
		}

	}
	else
	{
		FIntRect Rect(0, 0, SizeX, SizeY);
		//bScreenshotSuccessful = GetViewportScreenShot(Viewport, Bitmap, Rect);
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
		//const TCHAR* PNGFileName = TEXT("C:\\Users\\robert.pottorff\\Desktop\\output.png");
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

	/*
	UE_LOG(LogTemp, Warning, TEXT("Attempting capture!"));
	for (USceneCaptureComponent2D* camera :AAttachedSceneCaptureComponents2D) {
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
	*/
	FirstTime = false;
	return false;
}