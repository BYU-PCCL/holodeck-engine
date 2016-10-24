// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Messaging.h"
#include "HolodeckMessages.h"
#include "HolodeckPawnController.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECKCORE_API AHolodeckPawnController : public AAIController
{
	GENERATED_BODY()

public:
	AHolodeckPawnController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~AHolodeckPawnController();
	
	FRunnableThread* PublishingThread;
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;

	class Publisher : public FRunnable {
	private:
		AHolodeckPawnController* Controller;
	public:
		Publisher(AHolodeckPawnController*);
		virtual bool Init();
		virtual uint32 Run();
		virtual void Stop();
	};

	void Possess(APawn* InPawn);
	void UnPossess();

	void OnReceiveCommand(const FHolodeckUAVCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveCommand(const FHolodeckAndroidCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveCommand(const FHolodeckSphereRobotCommand& Command, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveConfiguration(const FHolodeckAndroidConfiguration& Configuration, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);
	void OnReceiveConfiguration(const FHolodeckUAVConfiguration& Configuration, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& Context);

	virtual void OnReceiveCommand(const FHolodeckUAVCommand& Command) { };
	virtual void OnReceiveCommand(const FHolodeckAndroidCommand& Command) { };
	virtual void OnReceiveCommand(const FHolodeckSphereRobotCommand& Command) { };
	virtual void OnReceiveConfiguration(const FHolodeckAndroidConfiguration& Configuration) { };
	virtual void OnReceiveConfiguration(const FHolodeckUAVConfiguration& Configuration) { };

	virtual void SendResponse(const FHolodeckResponse& Response) { };

	void Publish(FHolodeckSensorData Response);

};
