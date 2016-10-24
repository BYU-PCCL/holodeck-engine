// Fill out your copyright notice in the Description page of Project Settings.

#include "HolodeckPrivatePCH.h"
#include "HolodeckPawnController.h"


// TODO: Move publishing messages to a class that can be composed with actors who need additional controllers (like a PlayerController)

AHolodeckPawnController::AHolodeckPawnController(const FObjectInitializer& ObjectInitializer) : AAIController(ObjectInitializer) {
	MessageEndpoint = FMessageEndpoint::Builder("FHolodeckPawnControllerMessageEndpoint")
		.Handling<FHolodeckUAVCommand>(this, &AHolodeckPawnController::OnReceiveCommand)
		.Handling<FHolodeckAndroidCommand>(this, &AHolodeckPawnController::OnReceiveCommand)
		.Handling<FHolodeckSphereRobotCommand>(this, &AHolodeckPawnController::OnReceiveCommand)
		.Handling<FHolodeckAndroidConfiguration>(this, &AHolodeckPawnController::OnReceiveConfiguration)
		.Handling<FHolodeckUAVConfiguration>(this, &AHolodeckPawnController::OnReceiveConfiguration);

	if (MessageEndpoint.IsValid())
	{
		MessageEndpoint->Subscribe<FHolodeckUAVCommand>();
		MessageEndpoint->Subscribe<FHolodeckAndroidCommand>();
		MessageEndpoint->Subscribe<FHolodeckSphereRobotCommand>();
		MessageEndpoint->Subscribe<FHolodeckAndroidConfiguration>();
		MessageEndpoint->Subscribe<FHolodeckUAVConfiguration>();
		MessageEndpoint->Disable();
	}
	
}

AHolodeckPawnController::~AHolodeckPawnController() {
}

void AHolodeckPawnController::Possess(APawn* InPawn) {
	Super::Possess(InPawn);

	UE_LOG(LogHolodeck, Warning, TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Green, FString::Printf(TEXT("Pawn Possessed: %s, Controlled by: %s"), *InPawn->GetHumanReadableName(), *this->GetClass()->GetName()));

	if (MessageEndpoint.IsValid()) {
		MessageEndpoint->Enable();
	}
}

void AHolodeckPawnController::UnPossess() {
	Super::UnPossess();

	if (MessageEndpoint.IsValid()) {
		MessageEndpoint->Disable();
	}
}

void AHolodeckPawnController::OnReceiveCommand(const FHolodeckUAVCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	APawn* tester = this->GetPawn();
	FString human_readable = this->GetPawn()->GetHumanReadableName();
	if ((this->GetPawn() && Command.Target == this->GetPawn()->GetHumanReadableName())) {
		OnReceiveCommand(Command);
	}
};

void AHolodeckPawnController::OnReceiveCommand(const FHolodeckAndroidCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	if ((this->GetPawn() && Command.Target == this->GetPawn()->GetHumanReadableName())) {
		OnReceiveCommand(Command);
	}
};

void AHolodeckPawnController::OnReceiveCommand(const FHolodeckSphereRobotCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	if ((this->GetPawn() && Command.Target == this->GetPawn()->GetHumanReadableName())) {
		OnReceiveCommand(Command);
	}
};

void AHolodeckPawnController::OnReceiveConfiguration(const FHolodeckAndroidConfiguration& Configuration, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	if ((this->GetPawn() && Configuration.Target == this->GetPawn()->GetHumanReadableName())) {
		OnReceiveConfiguration(Configuration);
	}
};

void AHolodeckPawnController::OnReceiveConfiguration(const FHolodeckUAVConfiguration& Configuration, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context) {
	if ((this->GetPawn() && Configuration.Target == this->GetPawn()->GetHumanReadableName())) {
		OnReceiveConfiguration(Configuration);
	}
};

void AHolodeckPawnController::Publish(FHolodeckSensorData data) {
	FHolodeckResponse* response = new FHolodeckResponse();
	response->Source = this->GetPawn()->GetHumanReadableName(); //get tag 0 if it's there and use that instead of GetHumanReadableName()... otherwise just use GetHumanReadableName();

	response->Type = data.Type;
	response->Data = data.Data;

	MessageEndpoint->Publish<FHolodeckResponse>(response);
}