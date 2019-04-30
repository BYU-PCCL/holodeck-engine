#include "Holodeck.h"
#include "DistanceTask.h"
#include "Json.h"

// Set default values
void UDistanceTask::InitializeSensor() {
	Super::InitializeSensor();

	if (GoalObject) 
		StartDistance = (ProximityActor->GetActorLocation() - this->GetComponentLocation()).Size();
	else 
		StartDistance = (ProximityLocation - this->GetComponentLocation()).Size();

	NextDistance = StartDistance - Interval;
	LastDistance = StartDistance;
}

// Allows sensor parameters to be set programmatically from client.
void UDistanceTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::String>("ProximityActor")) {
			FString ActorName = JsonParsed->GetStringField("ProximityActor");
			for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->ActorHasTag(FName(*ActorName))) {
					ProximityActor = *ActorItr;
					break;
				}
			}
		}

		if (JsonParsed->HasTypedField<EJson::Array>("ProximityLocation")) {
			ProximityLocation = JsonParsed->GetBoolField("ProximityLocation");
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("UseSparseReward")) {
			UseSparseReward = JsonParsed->GetBoolField("UseSparseReward");
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
		if (UseSparseReward)
			SetSparceReward();
		else
			SetDenseReward();
	}

	// Call TaskSensor's Tick to store Reward and Terminal in sensor buffer
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}

// Sets the reward if UseDistanceReward is false
void UDistanceTask::SetDenseReward() {
	float Distance = (ProximityLocation - this->GetComponentLocation()).Size();
	if (GoalObject)
		Distance = (ProximityActor->GetActorLocation() - Parent->GetActorLocation()).Size();

	if (Distance < NextDistance) {
		Reward = 1;
		NextDistance = Distance - Interval;
	} else {
		Reward = 0;
	}

	Terminal = Distance < GoalDistance;
}

// Sets the reward if UseDistanceReward is true
void UDistanceTask::SetSparseReward() {
	float Distance = (ProximityLocation - this->GetComponentLocation()).Size();
	if (GoalObject)
		Distance = (ProximityActor->GetActorLocation() - Parent->GetActorLocation()).Size();

	if (Distance < GoalDistance) {
		Reward = 1;
		Terminal = true;
	} else {
		Reward = 0;
		Terminal = false;
	}
}
