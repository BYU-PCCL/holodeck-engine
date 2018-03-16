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
	for (int i = 0; i < Commands.size(); i++) {
		Commands[i]->Execute();
	}
	Commands.clear();

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

	}
	return Status;
}

void UCommandCenter::PrintJson(JsonValue Value) {
	//std::string MyString = Value.toString();
	//FString String = UTF8_TO_TCHAR(MyString.c_str());
	//UE_LOG(LogHolodeck, Log, TEXT("OutputString: %s"), *String);
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
		UE_LOG(LogHolodeck, Log, TEXT("Entering JSON_ARRAY"));
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

void UCommandCenter::ExctractCommandsFromJson(JsonValue Input){
	if (Input.getTag() == JSON_OBJECT) {
		for (auto Iter : Input) {
			JsonValue JsonCommand = Iter->value;
			if (JsonCommand.getTag() == JSON_OBJECT) {
				UE_LOG(LogHolodeck, Log, TEXT("Found a command and now Adding it to commands to execute."));
				this->GetCommand(JsonCommand);
			}
		}
	}
}

void UCommandCenter::GetCommand(JsonValue Input){
	//get the iterator, and then get the name of the command
	JsonIterator Iter = begin(Input);
	std::string CommandName = Iter->value.toString();
	
	//Now get the list of the parameters.
	std::vector<std::string> StringParameters;
	std::vector<float> FloatParameters;
	Iter.p = Iter->next; //Already got the frist value, and it was the name of the command. Start at the second. 
	while (Iter != nullptr) {
		if (Iter->value.getTag() == JSON_NUMBER) {
			FloatParameters.push_back(Iter->value.toNumber());
		} else{
			StringParameters.push_back(Iter->value.toString());
		}
		Iter.p = Iter->next;
	}

	//then just make the command!

}

//std::tuple<std::string, std::string> UCommandCenter::GetParameter(JsonValue Input){
//	return std::tuple<std::string, std::string>();
//}

//double UCommandCenter::sum_and_print(JsonValue o) {
	//double sum = 0;
	//switch (o.getTag()) {
	//case JSON_NUMBER:
	//	//UE_LOG(LogHolodeck, Log, TEXT("%f"), o.toNumber());
	//	sum += o.toNumber();
	//	break;
	//case JSON_STRING:
	//	//printf("\"%s\"\n", o.toString());
	//	//std::string MyString = o.toString();
	//	
	//	UE_LOG(LogHolodeck, Log, TEXT("MyString->"));
	//	break;
	//case JSON_ARRAY:
	//	for (auto i : o) {
	//		sum += sum_and_print(i->value);
	//	}
	//	break;
	//case JSON_OBJECT:
	//	for (auto i : o) {
	//		//UE_LOG(LogHolodeck, Log, TEXT("%s = "), i->key);
	//		sum += sum_and_print(i->value);
	//	}
	//	break;
	//case JSON_TRUE:
	//	UE_LOG(LogHolodeck, Log, TEXT("true"));
	//	break;
	//case JSON_FALSE:
	//	UE_LOG(LogHolodeck, Log, TEXT("false"));
	//	break;
	//case JSON_NULL:
	//	printf("null\n");
	//	UE_LOG(LogHolodeck, Log, TEXT("null"));
	//	break;
	//}
//	//return sum;
//}
