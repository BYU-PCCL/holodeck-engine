#include "Holodeck.h"
#include "LocationTask.h"
#include "Json.h"

// Set default values
void ULocationTask::InitializeSensor() {
	Super::InitializeSensor();
}

// Allows sensor parameters to be set programmatically from client.
void ULocationTask::ParseSensorParms(FString ParmsJson) {
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
				double X;
				double Y;
				double Z;
				if (LocationArray[0]->TryGetNumber(X) && LocationArray[1]->TryGetNumber(Y) && LocationArray[2]->TryGetNumber(Z))
					DistanceLocation = FVector(X, Y, Z);
			}
		}

		if (JsonParsed->HasTypedField<EJson::Number>("GoalDistance")) {
			GoalDistance = JsonParsed->GetNumberField("GoalDistance");
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("MaximizeDistance")) {
			MaximizeDistance = JsonParsed->GetBoolField("MaximizeDistance");
		}
	}
}

// Called every frame
void ULocationTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (IsValid(Parent)) {
		float Distance = (DistanceLocation - this->GetComponentLocation()).Size();
		if (DistanceActor)
			Distance = (DistanceActor->GetActorLocation() - Parent->GetActorLocation()).Size();

		if ((Distance > GoalDistance && MaximizeDistance) ||
				(Distance < GoalDistance && !MaximizeDistance)) {
			Reward = 1;
			Terminal = true;
		} else {
			Reward = 0;
			Terminal = false;
		}
	}

	// Call TaskSensor's Tick to store Reward and Terminal in sensor buffer
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
