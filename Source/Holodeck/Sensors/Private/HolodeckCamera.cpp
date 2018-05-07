#include "Holodeck.h"
#include "HolodeckCamera.h"

UHolodeckCamera::UHolodeckCamera() {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::UHolodeckCamer() initialization called."));
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	TargetTexture = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("TargetTexture"));
}

void UHolodeckCamera::BeginPlay() {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::BeginPlay"));
	Super::BeginPlay();

	//set up everything for the texture that you are using for output. These won't likely change for subclasses.
	//Note: the format should probably be 512 x 512 because it must be a square shape, and a power of two. If not, then the TargetTexture will cause crashes.
	TargetTexture->MipGenSettings = TMGS_NoMipmaps;
	TargetTexture->SRGB = false; //No alpha
	TargetTexture->CompressionSettings = TC_VectorDisplacementmap;
	TargetTexture->RenderTargetFormat = RTF_RGBA8;
	TargetTexture->InitCustomFormat(CaptureWidth, CaptureHeight, PF_FloatRGBA, false);

	//Handle whatever setup of the SceneCapture that won't likely change across different cameras. (These will be the defaults)
	SceneCapture->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCapture->CompositeMode = SCCM_Overwrite;
	SceneCapture->FOVAngle = 90; //90 degrees for field of view.
	SceneCapture->TextureTarget = TargetTexture;

	//The buffer has got to be an FColor pointer so you can export the pixel data to it. 
	this->Buffer = static_cast<FColor*>(Super::Buffer);

	this->ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
		if (ViewportClient != nullptr) {
			UE_LOG(LogHolodeck, Warning, TEXT("@@@HolodeckCAmera added itself to the viewport Camera list"));
			ViewportClient->AddCamera(this);
			this->bPointerGivenToViewport = true;
		}			
		else {
			UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::BeginPlay failed to locate HolodeckViewportClient."));
		}
	
}

void UHolodeckCamera::Capture() {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::Capture called"));


	AsyncTask(ENamedThreads::ActualRenderingThread, [&]()
	{
		bool bReadWorked = false;
		UE_LOG(LogHolodeck, Log, TEXT("THE THREAD WORKED! IT IS READING STUFF"));
		RenderTarget = TargetTexture->GetRenderTargetResource();
		UE_LOG(LogHolodeck, Log, TEXT("THE THREAD WORKED! IT IS READING STUFF step 2"));
		bReadWorked = RenderTarget->ReadPixelsPtr(this->Buffer);
		if (!bReadWorked) {
			UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::Capture() pixel read unsuccessful"));
		}
	});
	/*
	if (RenderTarget != nullptr) {
		
		


	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::Capture failed to retrieve TargetTexture's RenderTargetResource. Capture failed."));
	}
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::Capture ended"));*/
}

void UHolodeckCamera::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

}

//if (!bReadWorked) {
//	UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::Capture failed to read data to Buffer."));
//}