#include "Holodeck.h"
#include "DistanceTask.h"
#include "Json.h"

// Set default values
void UDistanceTask::InitializeSensor() {
	Super::InitializeSensor();

	if (DistanceActor) {
		StartDistance = (DistanceActor->GetActorLocation() - this->GetComponentLocation()).Size();
	} else {
		StartDistance = (DistanceLocation - this->GetComponentLocation()).Size();
	}

	if (MaximizeDistance) {
		NextDistance = StartDistance + Interval;
	} else {
		NextDistance = StartDistance - Interval;
	}
	LastDistance = StartDistance;
}

// Allows sensor parameters to be set programmatically from client.
void UDistanceTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::String>("DistanceActor")) {
			FString ActorName = JsonParsed->GetStringField("DistanceActor");
			for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->ActorHasTag(FName(*ActorName))) {
					DistanceActor = *ActorItr;
					break;
				}
			}
		}

		if (JsonParsed->HasTypedField<EJson::Array>("DistanceLocation")) {
			TArray<TSharedPtr<FJsonValue>> LocationArray = JsonParsed->GetArrayField("DistanceLocation");
			if (LocationArray.Num() == 3) {
				double X, Y, Z;
				if (LocationArray[0]->TryGetNumber(X) && LocationArray[1]->TryGetNumber(Y) && LocationArray[2]->TryGetNumber(Z))
					DistanceLocation = ConvertLinearVector(FVector(X, Y, Z), ClientToUE);
			}
		}

		if (JsonParsed->HasTypedField<EJson::Number>("Interval")) {
			Interval = ConvertClientDistanceToUnreal(JsonParsed->GetNumberField("Interval"));

		}

		if (JsonParsed->HasTypedField<EJson::Number>("GoalDistance")) {
			GoalDistance = ConvertClientDistanceToUnreal(JsonParsed->GetNumberField("GoalDistance"));
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("MaximizeDistance")) {
			MaximizeDistance = JsonParsed->GetBoolField("MaximizeDistance");
		}
	} else {
		UE_LOG(LogHolodeck, Warning, TEXT("UDistanceTask::ParseSensorParms:: Unable to parse json."));
	}
}

// Called every frame
void UDistanceTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (IsValid(Parent)) {
		float Distance = (DistanceLocation - this->GetComponentLocation()).Size();
		if (DistanceActor) {
			Distance = (DistanceActor->GetActorLocation() - Parent->GetActorLocation()).Size();
		}

		if (MaximizeDistance) {
			if (Distance > NextDistance) {
				Reward = 1;
				NextDistance = Distance + Interval;
			}
			else {
				Reward = 0;
			}
			Terminal = Distance > GoalDistance;
		}
		else {
			if (Distance < NextDistance) {
				Reward = 1;
				NextDistance = Distance - Interval;
			}
			else {
				Reward = 0;
			}
			Terminal = Distance < GoalDistance;
		}
	}

	// Call TaskSensor's Tick to store Reward and Terminal in sensor buffer
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
