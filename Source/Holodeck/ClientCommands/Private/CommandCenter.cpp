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
		BoolPtr = false;
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
		for (auto i : Value) {
			PrintJson(i->value);
		}
	}
		break;
	case JSON_OBJECT: {
		for (auto i : Value) {
			PrintJson(i->value);
		}
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
