// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HandAgentController.h"

AHandAgentController::AHandAgentController(const FObjectInitializer& ObjectInitializer) : AHolodeckPawnController(ObjectInitializer) {
	UE_LOG(LogHolodeck, Log, TEXT("Hand Agent Controller Initialized"));
}

void AHandAgentController::AddControlSchemes() {
	this->ControlScheme = NewObject<UJointMaxTorqueControlScheme>();
	this->ControlScheme->SetController(this);
	this->ControlScheme->SetControlSchemeSizeInBytes(AHandAgent::TOTAL_DOF);

	this->ControlScheme->SetJointSizes(
		AHandAgent::NUM_3_AXIS_JOINTS,
		AHandAgent::NUM_2_AXIS_JOINTS,
		AHandAgent::NUM_1_AXIS_JOINTS);

	// Note that the ControlScheme is not fully initialized until it has been provided
	// a skeletal mesh in ::Possess
	
	ControlSchemes.Add(ControlScheme);
}

void AHandAgentController::Possess(APawn* PawnParam) {
	Super::Possess(PawnParam);
	UE_LOG(LogHolodeck, Log, TEXT("Android Controller possessing pawn"));

	TArray<USkeletalMeshComponent*> Components;
	PawnParam->GetComponents<USkeletalMeshComponent>(Components);

	if (Components.Num() < 1) {
		UE_LOG(LogHolodeck, Fatal, TEXT("Couldn't find USkeletelMeshComponent for PawnParam"));
	}

	SkeletalMeshComponent = Components[0];
	this->ControlScheme->SetSkeletalMesh(SkeletalMeshComponent, AHandAgent::Joints);


	ActionBufferFloatPtr = static_cast<float*>(ActionBuffer);
}
