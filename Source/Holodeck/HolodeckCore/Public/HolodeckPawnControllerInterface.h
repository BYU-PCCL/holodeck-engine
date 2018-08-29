// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Holodeck.h"

#include "AIController.h"
#include "HolodeckServer.h"

#include "HolodeckPawnControllerInterface.generated.h"

/**
  * AHolodeckPawnControllerInterface
  * A controller for Holodeck Agents.
  * If a HolodeckAgent doesn't contain this controller or a controller which
  * inherits it, then you will run into problems. This class handles the 
  * subscribing of sensors and setting up action buffer channels. The 
  * sensors themselves tell the controller to subscribe them. Its base 
  * classes must report what size of action buffer they need. 
  * HolodeckPawnControllers get the data from the shared memory and give
  * the commands to the pawns/agents. 
  */
UCLASS()
class HOLODECK_API AHolodeckPawnControllerInterface : public AAIController
{
	GENERATED_BODY()

public:
	/**
	  * Default Constructor
	  */
	AHolodeckPawnControllerInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) :
		AAIController(ObjectInitializer) {};

	/**
	  * Default Destructor
	  */
	~AHolodeckPawnControllerInterface() {};

	/**
	* BeginPlay
	* Called when the game starts.
	* Registers the reward and terminal signals.
	*/
	virtual void BeginPlay() override {
		Super::BeginPlay();
	};

	/**
	* Tick
	* Ticks the agent.
	* If it is overridden, it must be called by the child class!
	* @param DeltaSeconds the time since the last tick.
	*/
	virtual void Tick(float DeltaSeconds) override {
		Super::Tick(DeltaSeconds);
	};
	
	/**
	  * Possess
	  * Called to Possess a pawn.
	  */
	virtual void Possess(APawn* InPawn) override {
		Super::Possess(InPawn);
	};

	/**
	  * UnPossess
	  * Called to Unpossess a pawn
	  */
	virtual void UnPossess() override {
		Super::UnPossess();
	};

	/**
	  * Subscribe
	  * Subscribes a sensor on the HolodeckServer.
	  * @param AgentName the name of the agent the sensor is being registered for.
	  * @param SensorName the name of the sensor to register.
	  * @param NumItems the number of items in the data buffer.
	  * @param ItemSize the size of each item in the data buffer.
	  * @return a pointer to the data buffer.
	  */
	virtual void* Subscribe(const FString& AgentName, const FString& SensorName, int NumItems, int ItemSize) {
		check(0 && "You must override Subscribe");
		return nullptr;
	};

	/**
	  * GetActionBuffer
	  * Gets the action buffer for this agent.
	  * @param AgentName the name of the agent to subscribe an action buffer for.

	  */
	virtual void AllocateBuffers(const FString& AgentName) {
		check(0 && "You must override AllocateBuffers");
	};

	virtual void AddControlSchemes() {
		check(0 && "You must override AddControlSchemes");
	};

	/**
	* ExecuteTeleport
	* Tells the controlled agent to teleport to the location in the shared memory.
	*/
	virtual void ExecuteTeleport() {
		check(0 && "You must override ExecuteTeleport");
	};

	/**
	* SetServer
	* Sets the server object within this object.
	*/
	virtual void SetServer(UHolodeckServer* const ServerParam) {
		check(0 && "You must override SetServer");
	};

	virtual void RestoreDefaultHyperparameters() {
		check(0 && "You must override RestoreDefaultHyperparameters");
	};
};
