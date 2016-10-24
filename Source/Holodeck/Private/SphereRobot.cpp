// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SphereRobot.h"


// Sets default values
ASphereRobot::ASphereRobot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ASphereRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASphereRobot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	controller = (AHolodeckPawnController*) this->GetController();

	reward_message = FHolodeckSensorData();
	reward_message.Data = FString::FromInt(Score);
	reward_message.Type = "Score";
	controller->Publish(reward_message);

	terminal_state_message = FHolodeckSensorData();
	terminal_state_message.Data = HasWon ? TEXT("true") : TEXT("false");
	terminal_state_message.Type = "Terminal";
	controller->Publish(terminal_state_message);


}

// Called to bind functionality to input
void ASphereRobot::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

