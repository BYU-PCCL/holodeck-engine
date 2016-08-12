// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"
#include "HolodeckMessages.h"
#include "SharedPointer.h"
#include "Networking.h"
#include "Sockets.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include <string>

#if PLATFORM_WINDOWS
// AllowedWindowsPlatformTypes.h prevents errors from being thrown
// by zmq.hpp using DWORD and other platform types
#include "AllowWindowsPlatformTypes.h" 
#include "zmq.hpp"
#include "zhelpers.hpp"
#include "HideWindowsPlatformTypes.h"
#elif PLATFORM_LINUX
#include "zmq.hpp"
#include "zhelpers.hpp"
#endif

#include "HolodeckServer.generated.h"

/**
 * The server recieves messages from an external source, deserializes them into appropriate
 * structs, and drops them onto a global message bus for other objects to use. It should be
 * instantiated only once by a persistent class such as GameSingleton or GameInstance
 */
UCLASS()
class HOLODECKCORE_API UHolodeckServer : public UObject
{

	GENERATED_BODY()

public:

	UHolodeckServer();
	~UHolodeckServer();

	/*
	 * Starts the server and binds to a particular port and creates a message endpoint.
	 * If the socket was successful, it starts a "listening thread" on which
	 * commands will be recieved
	 */
	void Start();

	/*
	 * Ends the server, and waits for the listening thread to stop before returning
	 */
	void End();

	/*
	 * Returns the running state of the server
	 */
	bool GetIsRunning();

private:
	TSharedPtr<zmq::context_t, ESPMode::ThreadSafe> Context;
	TSharedPtr<zmq::socket_t, ESPMode::ThreadSafe> Socket;

	FRunnableThread* ListeningThread;

	/** True if the server should allow the listening thread to continue running */
	bool IsRunning;

	/** A pointer to the local MessageEndpoint used to connect to the global message bus */
	TSharedPtr<FMessageEndpoint, ESPMode::ThreadSafe> MessageEndpoint;

	/** A CriticalSection used to prevent read/write errors */
	FCriticalSection SocketLock;

	class Listener : public FRunnable {
	public:
		Listener(UHolodeckServer*);

		/*
		 * Runs after the listening thread has been constructed
		 */
		virtual bool Init();

		/*
		 * The listening loop. This function loops as long as the server is running.
		 * It recives the two packets (identity and message) from the ZMQ socket. It uses
		 * a CriticalSection object owned by it's parent Server to lock and unlock the socket
		 * and prevent simultaenous read/write errors by the publisher.
		 */
		virtual uint32 Run();

		/*
		 * Runs after the listening thread has been stopped
		 */
		virtual void Stop();

	private:
		UPROPERTY()
			UHolodeckServer* Server;

	};

	/*
	 * The handler that is subscribed to the endpoint, this function is called when anyone publishes a HolodeckResponse message
	 * on the global message bus
	 */
	void RecieveResponse(const FHolodeckResponse& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& MessageContext);

	/*
	 * A helper function called by the listening thread after a message is recived. It parses the CommandMessage
	 * and then parses and publishes the Command via the ParseCommandAndPublish function.
	 */
	void HandleData(FString id, FString data);

	/*
	 * A Helper function to parse a CommandMessage (which contains a Command object as JSON) into the
	 * appropriate struct. The desired struct is supplied via the template.
	 */
	template <typename MessageType> bool ParseCommandAndPublish(FString& id, FHolodeckCommandMessage* ParsedMessage);
	
	/*
	 * Sends the message over the server to any connected clients
	 */
	bool Send(FHolodeckResponse Message);

	/*
	 * Locks a CriticalSection object that is used to prevent simultaneous read/write on a socket by two threads
	 */
	void LockSocket();

	/*
	 * Unlocks a CriticalSection object that is used to prevent simultaneous read/write on a socket by two threads
	 */
	void UnlockSocket();
};


