#include "Holodeck.h"
#include "CommandCenter.h"

void UCommandCenter::GiveCommand(std::unique_ptr<UCommand> &Input) {
	Commands.push_back(std::move(Input));
}

void UCommandCenter::GiveCommands(JsonValue Value) {

}

UCommandCenter::UCommandCenter() {
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter:: initialized"));
}

void UCommandCenter::Tick(float DeltaTime) {

	//read all of the commands in the buffer.
	// do not forget to terminate the source string with 0
	bool* BoolPtr = static_cast<bool*>(ShouldReadBufferPtr);
	if (BoolPtr && *BoolPtr == true) {
		UE_LOG(LogHolodeck, Log, TEXT("CommandCenter received signal to read from the readCommandBuffer"));
		ReadCommandBuffer();
		*BoolPtr = false;
		UE_LOG(LogHolodeck, Log, TEXT("CommandCenter set signal to false"));
	}
		
	//execute all of the commands that you found, plus any that were given to you.
	//for (int i = 0; i < Commands.size(); i++) {
	//	Commands[i]->Execute();
	//}
	//Commands.clear();

}

void UCommandCenter::GetCommandBuffer() {
		
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter:: is getting command buffer"));
	if (Server == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("CommandCenter could not find server..."));
		return;
	}
	else {
		Buffer = static_cast<char*>(Server->SubscribeSetting(TCHAR_TO_UTF8(*BUFFER_NAME), BUFFER_SIZE));
		ShouldReadBufferPtr = Server->SubscribeSetting(TCHAR_TO_UTF8(*BUFFER_SHOULD_READ_NAME), BUFFER_SHOULD_READ_SIZE * sizeof(bool));
		bool* BoolPtr = static_cast<bool*>(ShouldReadBufferPtr);
		*BoolPtr = false;
	}
}

void UCommandCenter::Init(UHolodeckServer* ParameterServer) {
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::init"));

	this->Server = ParameterServer;
	GetCommandBuffer();

}

int UCommandCenter::ReadCommandBuffer() {
	char *Endptr;
	JsonValue Value;
	JsonAllocator Allocator;
	int Status = jsonParse(Buffer, &Endptr, &Value, Allocator);
	if (Status != JSON_OK) {
		UE_LOG(LogHolodeck, Error, TEXT("Unable to parse command buffer as a json file"));
	} 
	else {
		PrintJson(Value);
		ExtractCommandsFromJson(Value);
	}
	return Status;
}

void UCommandCenter::ExtractCommandsFromJson(JsonValue Input){
	if (Input.getTag() == JSON_OBJECT) {
		//we are in the main object that hold the array of commands. 
		JsonIterator Iter = begin(Input);
		//Iter.p = Iter->next; //maybe I dont actually have to get the next one when I make an iter? 
		//check if this is actually that array, and then extract the commands from it.
		if (Iter->value.getTag() == JSON_ARRAY) {
			//We are in the array of commands. 
			for (JsonNode* ArrayIter : Iter->value) {
				//get the command!!
				GetCommand(ArrayIter->value);
			}
		} 
		else {
			UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));
		}
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));

	}
}

void UCommandCenter::GetCommand(JsonValue Input){
	//get the iterator, and then get the name of the command
	JsonIterator Iter = begin(Input);
	std::string CommandName = Iter->value.toString();
	FString CommandFString = UTF8_TO_TCHAR(CommandName.c_str());
	UE_LOG(LogHolodeck, Log, TEXT("CommandName: %s"), *CommandFString);
	//Now to get the list of the parameters...
	std::vector<std::string> StringParameters;
	std::vector<float> FloatParameters;
	Iter.p = Iter->next; //Already got the frist value, and it was the name of the command. Go to the second. This should be the array of parameters
	bool bSucceeded = true;
	if (Iter->value.getTag() == JSON_ARRAY) {
		//This means that we are in the array
		for (JsonNode* ArrayIter : Iter->value) {
			//the value inside ArrayIter will be a JSON object, we need to get the value wrapped inside it. 
			JsonIterator ObjIter = begin(ArrayIter->value);
				if (ObjIter->value.getTag() == JSON_NUMBER) {
					FloatParameters.push_back(ObjIter->value.toNumber());
				} 
				else{
					StringParameters.push_back(ObjIter->value.toString());
				}
		}
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("Command Buffer didn't contain the format of JSON we expected. Unable to process command."));
		bSucceeded = false;
	}

	//then just make and give the command!
	std::unique_ptr<UCommand> CommandPtr = std::move(UCommandFactory::MakeCommand(CommandName, FloatParameters, StringParameters));
	this->GiveCommand(CommandPtr);
}

void UCommandCenter::PrintJson(JsonValue Value) {
	switch (Value.getTag()) {
	case JSON_NUMBER: {
		UE_LOG(LogHolodeck, Log, TEXT("%f"), Value.toNumber());
	}
					  break;
	case JSON_STRING: {

		std::string MyString = Value.toString();
		FString String = UTF8_TO_TCHAR(MyString.c_str());

		UE_LOG(LogHolodeck, Log, TEXT("OutputString: %s"), *String);
	}
					  break;
	case JSON_ARRAY: {
		UE_LOG(LogHolodeck, Log, TEXT("[[[Entering JSON_ARRAY]]]"));
		for (auto i : Value) {
			UE_LOG(LogHolodeck, Log, TEXT("JSON value of entered array:"));
			PrintJson(i->value);
		}
	}
					 break;
	case JSON_OBJECT: {
		UE_LOG(LogHolodeck, Log, TEXT("@@@Entering JSON_OBJECT@@@"));
		for (auto i : Value) {
			UE_LOG(LogHolodeck, Log, TEXT("JSON value of entered object:"));
			PrintJson(i->value);
		}
		UE_LOG(LogHolodeck, Log, TEXT("@@@Exiting JSON_OBJECT@@@"));
	}
					  break;
	case JSON_TRUE: {
		UE_LOG(LogHolodeck, Log, TEXT("true"));
	}
					break;
	case JSON_FALSE: {
		UE_LOG(LogHolodeck, Log, TEXT("false"));
	}
					 break;
	case JSON_NULL: {
		UE_LOG(LogHolodeck, Log, TEXT("null"));
	}
					break;
	}
}
