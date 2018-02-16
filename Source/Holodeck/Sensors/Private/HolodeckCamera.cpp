#include "Holodeck.h"
#include "HolodeckCamera.h"

UHolodeckCamera::UHolodeckCamera() {
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	TargetTexture = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("TargetTexture"));
}

void UHolodeckCamera::BeginPlay() {
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
		ViewportClient->AddCamera(this);
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::BeginPlay failed to locate HolodeckViewportClient."));
	}
}

bool UHolodeckCamera::Capture() {
	FTextureRenderTargetResource* RenderTarget = TargetTexture->GetRenderTargetResource();
	bool bReadWorked = false;
	if (RenderTarget != nullptr) {
		bReadWorked = RenderTarget->ReadPixelsPtr(this->Buffer);//, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(0, 0, Width, Height))
		if (!bReadWorked) {
			UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::Capture failed to read data to Buffer."));
		}
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("UHolodeckCamera::Capture failed to retrieve TargetTexture's RenderTargetResource. Capture failed."));
	}
	return bReadWorked;
}

