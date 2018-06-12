#include "Holodeck.h"
#include "RenderRequest.h"

void FRenderRequest::RetrievePixels(FColor* PixelBuffer, UTextureRenderTarget2D* Texture) {
	this->Buffer = PixelBuffer;
	this->TargetTexture = Texture;
	CheckNotBlockedOnRenderThread(); // not the issue

	// Queue up the task of rendering the scene in the render thread
	TGraphTask<FRenderRequest>::CreateTask().ConstructAndDispatchWhenReady(*this);
}

void FRenderRequest::ExecuteTask()
{
	TArray<FColor> SurfaceData;
	FRHICommandListImmediate& RHICmdList = GetImmediateCommandList_ForRenderCommand();
	FTextureRenderTargetResource*  rt_resource = TargetTexture->GetRenderTargetResource();

	if (rt_resource != nullptr) {
		const FTexture2DRHIRef& rhi_texture = rt_resource->GetRenderTargetTexture();
		FIntPoint size;
		FReadSurfaceDataFlags flags(RCM_UNorm, CubeFace_MAX);
		flags.SetLinearToGamma(false);
		RHICmdList.ReadSurfaceData(
			rhi_texture,
			FIntRect(0, 0, TargetTexture->SizeX, TargetTexture->SizeY),
			SurfaceData,
			flags);

		FMemory::Memcpy(this->Buffer, &SurfaceData[0], SurfaceData.Num() * sizeof(FColor)); // this line isn't the problem
	}
}
