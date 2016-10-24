// Fill out your copyright notice in the Description page of Project Settings.

#include "HolodeckPrivatePCH.h"
#include "HolodeckServer.h"

UHolodeckServer::UHolodeckServer(){
	// Warning -- This class gets initialized a few times by Unreal because it is a UObject. 
	// DO NOT rely on singleton-qualities in the constructor, only in the Start() function
}

UHolodeckServer::~UHolodeckServer() {
	delete ListeningThread;
	ListeningThread = NULL;
}


void UHolodeckServer::Start() {
	// A boolean to let the listening thread know it should keep running,
	// or stop
	IsRunning = true;

	Context = TSharedPtr<zmq::context_t, ESPMode::ThreadSafe>(new zmq::context_t(1));
	Socket = TSharedPtr<zmq::socket_t, ESPMode::ThreadSafe>(new zmq::socket_t(*Context, ZMQ_ROUTER));

	MessageEndpoint = FMessageEndpoint::Builder("FMessageHolodeckServer").Handling<FHolodeckResponse>(this, &UHolodeckServer::RecieveResponse);

	// Actually listen to the incomming messages
	if (Socket.IsValid() && MessageEndpoint.IsValid())
	{
		// Get the port number from the Engine.ini file
		// or use a default
		FString PortNumber = "8989";
		if (GConfig) {
			FString SettingsPortNumber;
			GConfig->GetString(
				TEXT("/Holodeck/Settings.Server"),
				TEXT("ListeningPort"),
				SettingsPortNumber,
				GEngineIni
				);
			if (!SettingsPortNumber.IsEmpty()) {
				PortNumber = SettingsPortNumber;
			}
		}

		// Build the URL
		FString ConnectURL = FString::Printf(TEXT("tcp://*:%s"), *PortNumber);

		UE_LOG(LogHolodeck, Warning, TEXT("Server started at %s"), *ConnectURL);
		GEngine->AddOnScreenDebugMessage(-1, 100, FColor::Green, FString::Printf(TEXT("Server started at %s"), *ConnectURL));


		// Bind the server to the socket
		Socket->bind(TCHAR_TO_ANSI(*ConnectURL));

		// Subscribe the server to listen for HolodeckResponses
		MessageEndpoint->Subscribe<FHolodeckResponse>();

		// Start the listening thread at Normal priority
		ListeningThread = FRunnableThread::Create(new UHolodeckServer::Listener(this), TEXT("FServerReceiver"), 0, TPri_Normal);
	}
}


void UHolodeckServer::End() {
	IsRunning = false;

	ListeningThread->WaitForCompletion();
}


bool UHolodeckServer::GetIsRunning() {
	return IsRunning;
}


void UHolodeckServer::RecieveResponse(const FHolodeckResponse& Message, const TSharedRef<IMessageContext, ESPMode::ThreadSafe>& MessageContext) {
	Send(Message);
}


bool UHolodeckServer::Send(FHolodeckResponse Message) {
	FString json;
	FJsonObjectConverter::UStructToJsonObjectString(Message.StaticStruct(), &Message, json, 0, 0);

	// Lock the socket to prevent race conditions 
	// with the listenting thread
	LockSocket();

	s_sendmore(*Socket, TCHAR_TO_UTF8(*Message.Source));
	s_send(*Socket, TCHAR_TO_UTF8(*json));

	UnlockSocket();

	return true;
}


void UHolodeckServer::LockSocket() {
	SocketLock.Lock();
}


void UHolodeckServer::UnlockSocket() {
	SocketLock.Unlock();
}


template <typename MessageType>
bool UHolodeckServer::ParseCommandAndPublish(FString& id, FHolodeckCommandMessage* ParsedMessage) {
	MessageType *ParsedCommand = new MessageType(); // MessageEndpoint takes control of the ParsedCommand pointer

	if (FJsonObjectConverter::JsonObjectStringToUStruct<MessageType>(ParsedMessage->CommandJSON, ParsedCommand, 0, 0)) { //Take the contents of ParsedMessage->CommandJSON and put them in ParsedCommad
		ParsedCommand->Target = id;

		MessageEndpoint->Publish<MessageType>(ParsedCommand);

		return true;
	}

	return false;
}


void UHolodeckServer::HandleData(FString id, FString data) {
	FHolodeckCommandMessage* ParsedMessage = new FHolodeckCommandMessage();
	bool success = false;

	if (FJsonObjectConverter::JsonObjectStringToUStruct<FHolodeckCommandMessage>(data, ParsedMessage, 0, 0)) {

		// Set the Target as the identity passed over the socket
		ParsedMessage->Target = id;

		if (ParsedMessage->CommandType == TEXT("AndroidCommand")){
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to android");
			success = ParseCommandAndPublish<FHolodeckAndroidCommand>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("AndroidConfiguration")) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to android config");
			success = ParseCommandAndPublish<FHolodeckAndroidConfiguration>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("UAVCommand")) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to uav");
			success = ParseCommandAndPublish<FHolodeckUAVCommand>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("UAVConfiguration")) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to uav config");
			success = ParseCommandAndPublish<FHolodeckUAVConfiguration>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("SimulatorCommand")) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to simulator");
			success = ParseCommandAndPublish<FHolodeckSimulatorCommand>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("SphereRobotCommand")) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to simulator");
			success = ParseCommandAndPublish<FHolodeckSphereRobotCommand>(id, ParsedMessage);
		}
		else if (ParsedMessage->CommandType == TEXT("WaitForConnect")) {
			// If we have recieved the message, it means we are connected
			// so send a success response
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sending message to WaitForConnect... ");
			FHolodeckResponse Response = FHolodeckResponse();
			Response.Type = TEXT("Connect");
			Response.Source = id;
			Response.Data = TEXT("Success");
			Send(Response);
			success = true;
		}

	}

	if (!success) {
		UE_LOG(LogHolodeck, Error, TEXT("Error deserializing, Command type not found, or invalid command body : %s"), *data);
	}
	else {
		MessageEndpoint->Publish<FHolodeckCommandMessage>(ParsedMessage);
	}
		
}


UHolodeckServer::Listener::Listener(UHolodeckServer* serverPointer) {
	Server = serverPointer;
}


bool UHolodeckServer::Listener::Init() {
	return true;
};


uint32 UHolodeckServer::Listener::Run() {
	zmq::message_t identity;
	zmq::message_t request;
	FString data;
	FString id;
	bool received = false;

	while (Server->GetIsRunning() && Server->Socket.IsValid()) {

		Server->LockSocket();
		if (Server->Socket->recv(&identity, ZMQ_NOBLOCK)) {
			Server->Socket->recv(&request);
			received = true; 
		}
		Server->UnlockSocket();

		// Use the received bool to defer any non-socket processing until after unlocking
		if (received) {
			id = FString(std::string(static_cast<char*>(identity.data()), identity.size()).c_str());
			data = FString(std::string(static_cast<char*>(request.data()), request.size()).c_str());
			Server->HandleData(id, data);
			received = false;
		}
		
	}

	return 0;
};


void UHolodeckServer::Listener::Stop() {
};