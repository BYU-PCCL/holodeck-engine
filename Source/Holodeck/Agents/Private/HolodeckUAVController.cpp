// Fill out your copyright notice in the Description page of Project Settings.
#include "Holodeck.h"
#include "UAV.h"
#include "HolodeckMessages.h"
#include "HolodeckUAVController.h"
AHolodeckUAVController::AHolodeckUAVController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {

	UE_LOG(LogTemp, Warning, TEXT("UAV Controller Initialized"));
}
AHolodeckUAVController::~AHolodeckUAVController() {
}
void AHolodeckUAVController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);
	//TArray<USkeletalMeshComponent*> components;
	//Pawn->GetComponents<USkeletalMeshComponent>(components);
	//SkeletalMeshComponent = components[0];
}
void AHolodeckUAVController::OnReceiveCommand(const FHolodeckUAVCommand& Command) {

	//if (Command.LocalRoll != 0 && Command.LocalPitch != 0 && Command.LocalYaw != 0) {
	//  this->GetPawn()->AddActorLocalRotation(FRotator(Command.LocalRoll, Command.LocalPitch, Command.LocalYaw));
	//}
	AUAV* Pawn = Cast<AUAV>(GetPawn());
	Pawn->DesiredRoll = Command.Roll;
	Pawn->DesiredPitch = Command.Pitch;
	Pawn->DesiredYawRate = Command.YawRate;
	Pawn->DesiredAltitude = Command.Altitude;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, this->GetPawn()->GetActorRotation().ToString());
	//(Command.LocalRoll, Command.LocalPitch, Command.LocalYawVel, Command.Thrust)

}
void AHolodeckUAVController::OnReceiveConfiguration(const FHolodeckUAVConfiguration& Configuration) {

	//UE_LOG(LogTemp, Warning, TEXT("UAV Configuration Receieved"));
}
void AHolodeckUAVController::tick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Tick");
}