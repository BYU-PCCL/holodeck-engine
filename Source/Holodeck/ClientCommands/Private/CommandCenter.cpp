#include "Holodeck.h"
#include "CommandCenter.h"
#include "HolodeckGameMode.h" // to avoid a circular dependency. 

const FString UCommandCenter::BUFFER_NAME = "command_buffer";
const FString UCommandCenter::BUFFER_SHOULD_READ_NAME = "command_bool";
const int UCommandCenter::BUFFER_SHOULD_READ_SIZE = 1;
const int UCommandCenter::BUFFER_SIZE = 1048576; //one megabyte
const int UCommandCenter::BYTE_SIZE = 8;

void UCommandCenter::GiveCommand(UCommand * const Input) {
	if (Input != nullptr)
		Commands.Add(Input);
}

UCommandCenter::UCommandCenter() {
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::CommandCenter() constructed"));
}

void UCommandCenter::Tick(float DeltaTime) {
	if (ShouldReadBufferPtr && *ShouldReadBufferPtr == true) {
		ReadCommandBuffer();
		*ShouldReadBufferPtr = false;
	}
	for (const auto &i : Commands)
		i->Execute();
	Commands.Empty();
}

void UCommandCenter::GetCommandBuffer() {
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter:: is getting command buffer"));
	if (Server == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("CommandCenter could not find server..."));
	} else {
		Buffer = static_cast<char*>(Server->SubscribeSetting(TCHAR_TO_UTF8(*BUFFER_NAME), BUFFER_SIZE * BYTE_SIZE));
		ShouldReadBufferPtr = static_cast<bool*>(Server->SubscribeSetting(TCHAR_TO_UTF8(*BUFFER_SHOULD_READ_NAME), BUFFER_SHOULD_READ_SIZE * sizeof(bool)));
		if (ShouldReadBufferPtr != nullptr)
			*ShouldReadBufferPtr = false;
		else
			UE_LOG(LogHolodeck, Error, TEXT("UCommandCenter::ShouldReadBufferPtr is null"));
	}
}

void UCommandCenter::Init(UHolodeckServer* const ParameterServer, AHolodeckGameMode* const ParameterGameMode) {
	this->Server = ParameterServer;
	this->GameMode = ParameterGameMode;
	GetCommandBuffer();
}

int UCommandCenter::ReadCommandBuffer() {
	char *Endptr;
	JsonValue Value;
	JsonAllocator Allocator;
	int Status = jsonParse(Buffer, &Endptr, &Value, Allocator);
	if (Status != JSON_OK) {
		UE_LOG(LogHolodeck, Error, TEXT("Unable to parse command buffer as a json file"));
	} else {
		ExtractCommandsFromJson(Value);
	}
	return Status;
}

void UCommandCenter::ExtractCommandsFromJson(const JsonValue &Input){
	if (Input.getTag() == JSON_OBJECT) {
		JsonIterator Iter = begin(Input);
		//check if this is actually the array of commands, and then extract the commands from it.
		if (Iter->value.getTag() == JSON_ARRAY) {
			for (JsonNode* ArrayIter : Iter->value)
				GetCommand(ArrayIter->value);
		} else {
			UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));
		}
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));
	}
}

void UCommandCenter::GetCommand(const JsonValue &Input) {
	JsonIterator Iter = begin(Input);
	std::string CommandName = Iter->value.toString();
	FString CommandFString = UTF8_TO_TCHAR(CommandName.c_str());
	std::vector<std::string> StringParameters;
	std::vector<float> FloatParameters;
	Iter.p = Iter->next;
	if (Iter->value.getTag() == JSON_ARRAY) {
		//We are now inside the array
		for (JsonNode* ArrayIter : Iter->value) {
			JsonIterator ObjIter = begin(ArrayIter->value);
			if (ObjIter->value.getTag() == JSON_NUMBER)
				FloatParameters.push_back(ObjIter->value.toNumber());
			else
				StringParameters.push_back(ObjIter->value.toString());
		}
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));
	}
	UCommand* CommandPtr = UCommandFactory::MakeCommand(CommandName, FloatParameters, StringParameters, GameMode);
	this->GiveCommand(CommandPtr);
}

void const UCommandCenter::PrintJson(const JsonValue& Value) {
	std::string MyString;
	FString String;
	switch (Value.getTag()) {
	case JSON_NUMBER:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("%f"), Value.toNumber());
		break;
	case JSON_STRING: 
		MyString = Value.toString();
		String = UTF8_TO_TCHAR(MyString.c_str());
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("OutputString: %s"), *String);
		break;
	case JSON_ARRAY:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("[[[Entering JSON_ARRAY]]]"));
		for (const auto& i : Value) {
			UE_LOG(LogHolodeck, VeryVerbose, TEXT("JSON value of entered array:"));
			PrintJson(i->value);
		}
		break;
	case JSON_OBJECT:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("@@@Entering JSON_OBJECT@@@"));
		for (auto i : Value) {
			UE_LOG(LogHolodeck, VeryVerbose, TEXT("JSON value of entered object:"));
			PrintJson(i->value);
		}
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("@@@Exiting JSON_OBJECT@@@"));
		break;
	case JSON_TRUE:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("true"));
		break;
	case JSON_FALSE:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("false"));
		break;
	case JSON_NULL:
		UE_LOG(LogHolodeck, VeryVerbose, TEXT("null"));
		break;
	}
}
