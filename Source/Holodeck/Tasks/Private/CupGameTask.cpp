// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "HolodeckGameMode.h"
#include "CupGameTask.h"


void UCupGameTask::InitializeSensor() {
	Super::InitializeSensor();
}

// Allows sensor parameters to be set programmatically from client.
void UCupGameTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);

	int32 Speed = 1;
	int32 NumShuffles = 3;
	bool UseSeed = false;
	int32 Seed = 0;

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::Number>("Speed")) {
			Speed = JsonParsed->GetNumberField("Speed");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("NumShuffles")) {
			NumShuffles = JsonParsed->GetNumberField("NumShuffles");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("Seed")) {
			UseSeed = true;
			Seed = JsonParsed->GetNumberField("Seed");
		}

		TArray<float> nums;
		nums.Add(Speed);
		nums.Add(NumShuffles);
		nums.Add(UseSeed);
		nums.Add(Seed);
		TArray<FString> strs;
		AActor* Target = GetWorld()->GetAuthGameMode();
		AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
		Game->ExecuteCustomCommand("CupGameConfig", nums, strs);
		Game->ExecuteCustomCommand("StartCupGame", nums, strs);
	}
	else {
		UE_LOG(LogHolodeck, Warning, TEXT("UCupGameTask::ParseSensorParms:: Unable to parse json."));
	}
}

void UCupGameTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Reward = 10;
	AActor* Target = GetWorld()->GetAuthGameMode();
	AHolodeckGameMode* Game = static_cast<AHolodeckGameMode*>(Target);
	int32 BallLocation = (int32)Game->GetWorldNum("BallLocation");

}
