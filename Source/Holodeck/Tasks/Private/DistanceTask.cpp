#include "Holodeck.h"
#include "DistanceTask.h"
#include "Json.h"

// Set default values
void UDistanceTask::InitializeSensor() {
	Super::InitializeSensor();

	StartDistance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size();
	NextDistance = StartDistance - Interval;
	LastDistance = StartDistance;
}

// Allows sensor parameters to be set programmatically from client.
void UDistanceTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::String>("GoalObject")) {
			FString ActorName = JsonParsed->GetStringField("GoalObject");
			for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->GetName() == ActorName) {
					GoalObject = *ActorItr;
					break;
				}
			}
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("UseDistanceReward")) {
			UseDistanceReward = JsonParsed->GetBoolField("UseDistanceReward");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("Interval")) {
			Interval = JsonParsed->GetNumberField("Interval");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("GoalDistance")) {
			GoalDistance = JsonParsed->GetNumberField("GoalDistance");
		}
	}
}

// Called every frame
void UDistanceTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (IsValid(Parent)) {
		if (UseDistanceReward)
			SetDistanceReward();
		else
			SetUnitReward();
	}

	// Call TaskSensor's Tick to store Reward and Terminal in sensor buffer
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}

// Sets the reward if UseDistanceReward is false
void UDistanceTask::SetUnitReward() {
	float Distance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size();
	if (Distance < NextDistance) {
		Reward = 1;
		NextDistance = Distance - Interval;
	} else {
		Reward = -1;
	}
	Terminal = Distance < GoalDistance;
}

// Sets the reward if UseDistanceReward is true
void UDistanceTask::SetDistanceReward() {
	float Distance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size() / StartDistance;
	Reward = LastDistance - Distance;
	Terminal = Distance < GoalDistance;
	LastDistance = Distance;
}
