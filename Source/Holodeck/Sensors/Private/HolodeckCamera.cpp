#include "Holodeck.h"
#include "HolodeckCamera.h"

UHolodeckCamera::UHolodeckCamera() {

}

void UHolodeckCamera::BeginPlay() {
	Super::BeginPlay();
	this->ViewportClient = Cast<UHolodeckViewportClient>(GEngine->GameViewport);
	if (ViewportClient != nullptr) {
		ViewportClient->AddCamera(this);
	}
}
