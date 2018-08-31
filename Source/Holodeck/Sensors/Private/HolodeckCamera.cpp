#include "Holodeck.h"
#include "HolodeckCamera.h"

UHolodeckCamera::UHolodeckCamera() {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::UHolodeckCamer() initialization called."));

	int CameraWidth;
	int CameraHeight;
	if(FParse::Value(FCommandLine::Get(), TEXT("CamResX="), CameraWidth)) {
		CaptureWidth = CameraWidth;
	}
	if (FParse::Value(FCommandLine::Get(), TEXT("CamResY="), CameraHeight)) {
		CaptureHeight = CameraHeight;
	}

	UE_LOG(LogHolodeck, Log, TEXT("CaptureHeight is %d"), CaptureHeight);
	UE_LOG(LogHolodeck, Log, TEXT("CaptureWidth is %d"), CaptureWidth);

	SceneCapture = this->CreateDefaultSubobject<USceneCaptureComponent2D>("SceneCap");
	SceneCapture->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

void UHolodeckCamera::BeginPlay() {
	UE_LOG(LogHolodeck, Log, TEXT("UHolodeckCamera::BeginPlay"));
	Super::BeginPlay();
	
	TargetTexture = NewObject<UTextureRenderTarget2D>(this);

	RenderRequest = FRenderRequest();
	
	//set up everything for the texture that you are using for output. These won't likely change for subclasses.
	//Note: the format should probably be 512 x 512 because it must be a square shape, and a power of two. If not, then the TargetTexture will cause crashes.
	TargetTexture->SRGB = false; //No alpha
	TargetTexture->CompressionSettings = TC_VectorDisplacementmap;
	TargetTexture->RenderTargetFormat = RTF_RGBA8;
	TargetTexture->InitCustomFormat(CaptureWidth, CaptureHeight, PF_FloatRGBA, false);

	//Handle whatever setup of the SceneCapture that won't likely change across different cameras. (These will be the defaults)
	SceneCapture->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCapture->CompositeMode = SCCM_Overwrite;
	SceneCapture->FOVAngle = 90; //90 degrees for field of view.
	SceneCapture->CaptureSource = SCS_SceneColorHDR;
	SceneCapture->TextureTarget = TargetTexture;
	SceneCapture->PostProcessSettings.bOverride_AutoExposureBias = 1;

	// Higher = brighter captured image. Lower = darker
	SceneCapture->PostProcessSettings.AutoExposureBias = 2.5;

	//The buffer has got to be an FColor pointer so you can export the pixel data to it. 
	this->Buffer = static_cast<FColor*>(Super::Buffer);
	this->ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
}

void UHolodeckCamera::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	RenderRequest.RetrievePixels(Buffer, TargetTexture);
}