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
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::tick"));

	//read all of the commands in the buffer.
	// do not forget to terminate the source string with 0
	bool* BoolPtr = static_cast<bool*>(ShouldReadBufferPtr);
	if (BoolPtr && *BoolPtr == true) {
		ReadCommandBuffer();
		BoolPtr = false;
	}
	
	//execute all of the commands that you found, plus any that were given to you.
	for (int i = 0; i < Commands.size(); i++) {
		Commands[i]->Execute();
	}
	Commands.clear();
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::tick end"));

}

void UCommandCenter::GetCommandBuffer() {
		
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter:: is getting command buffer"));
	if (Server == nullptr) {
		UE_LOG(LogHolodeck, Warning, TEXT("CommandCenter could not find server..."));
		return;
	}
	else {
		Buffer = static_cast<char*>(Server->SubscribeSetting(BUFFER_NAME, BUFFER_SIZE));
		ShouldReadBufferPtr = Server->SubscribeSetting(BUFFER_SHOULD_READ_NAME, BUFFER_SHOULD_READ_SIZE);
	}
}

void UCommandCenter::Init(UHolodeckServer* ParameterServer) {
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::init"));

	this->Server = ParameterServer;
	GetCommandBuffer();
	UE_LOG(LogHolodeck, Log, TEXT("CommandCenter::init end"));

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
		sum_and_print(Value);
	}
	return Status;
}

double UCommandCenter::sum_and_print(JsonValue o) {
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
	//return sum;
}
