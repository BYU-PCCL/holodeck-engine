#include "Holodeck.h"
#include "PixelCamera.h"

UPixelCamera::UPixelCamera() {

}


void UPixelCamera::InitializeSensor() {
	Super::InitializeSensor();

	//Set up everything for the scenecapturecomponent2d
	SceneCapture->CaptureSource = SCS_FinalColorLDR; //Pick what type of output you want to be sent to the texture target. 	
}
