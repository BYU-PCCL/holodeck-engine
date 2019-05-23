#include "Holodeck.h"
#include "AvoidTask.h"
#include "Json.h"

// Set default values
void UAvoidTask::InitializeSensor() {
	Super::InitializeSensor();
}

// Allows sensor parameters to be set programmatically from client.
void UAvoidTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::String>("ToAvoid")) {
			ToAvoidTag = JsonParsed->GetStringField("ToAvoid");
		}

		if (JsonParsed->HasTypedField<EJson::String>("StartSocket")) {
			StartSocket = JsonParsed->GetStringField("StartSocket");
		}

		if (JsonParsed->HasTypedField<EJson::String>("EndSocket")) {
			EndSocket = JsonParsed->GetStringField("EndSocket");
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("OnlyWithinSight")) {
			OnlyWithinSight = JsonParsed->GetBoolField("OnlyWithinSight");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("FOVRadians")) {
			FOVRadians = JsonParsed->GetNumberField("FOVRadians");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("MinDistance")) {
			MinDistance = JsonParsed->GetNumberField("MinDistance");
		}
	}
}

// Called every frame
void UAvoidTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (ToAvoid == nullptr && ToAvoidTag != "")
		ToAvoid = FindActorWithTag(ToAvoidTag);

	if (ToAvoid) {
		// Get location and distance
		FVector StartVec = ToAvoid->GetActorLocation();
		UStaticMeshComponent* Mesh = (UStaticMeshComponent*)ToAvoid->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
		if (Mesh && Mesh->DoesSocketExist(FName(*StartSocket))) {
			StartVec = Mesh->GetSocketLocation(FName(*StartSocket));
		}

		FVector EndVec = Parent->GetActorLocation();
		Mesh = (UStaticMeshComponent*)Parent->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
		if (Mesh && Mesh->DoesSocketExist(FName(*EndSocket))) {
			EndVec = Mesh->GetSocketLocation(FName(*EndSocket));
		}

		FVector DistanceVec = EndVec - StartVec;
		float Distance = DistanceVec.Size();

		if (OnlyWithinSight) {
			// Get angle to target
			float TargetAngle = FGenericPlatformMath::Acos(FVector::DotProduct(DistanceVec / Distance, ToAvoid->GetActorForwardVector()));

			// Get trace to target
			FCollisionQueryParams QueryParams = FCollisionQueryParams();
			QueryParams.AddIgnoredActor(ToAvoid);
			FHitResult Hit = FHitResult();
			bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartVec, EndVec, ECollisionChannel::ECC_Visibility, QueryParams);

			// Evaluate - if the actor is in our field of view and either the ray trace has intersected with the target or there is nothing between ourself and the target
			if (TargetAngle < FOVRadians && (Hit.Actor == Parent || Hit.Actor == nullptr) && Distance < MinDistance) {
				Reward = MaxScore * Distance / MinDistance;
			}
			else {
				Reward = 100;
			}
		}
		else {
			if (Distance < MinDistance) {
				Reward = MaxScore * Distance / MinDistance;
			}
			else {
				Reward = 100;
			}
		}
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
