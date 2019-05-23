#include "Holodeck.h"
#include "FollowTask.h"
#include "Json.h"

// Set default values
void UFollowTask::InitializeSensor() {
	Super::InitializeSensor();
}

// Allows sensor parameters to be set programmatically from client.
void UFollowTask::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::String>("ToFollow")) {
			ToFollowTag = JsonParsed->GetStringField("ToFollow");
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

		if (JsonParsed->HasTypedField<EJson::Number>("TargetHeight")) {
			TargetHeight = JsonParsed->GetNumberField("TargetHeight");
		}
	}
}

// Called every frame
void UFollowTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (ToFollow == nullptr && ToFollowTag != "")
		ToFollow = FindActorWithTag(ToFollowTag);

	if (ToFollow) {
		// Get location and distance
		FVector AgentLocation = Parent->GetActorLocation();
		FVector TargetLocation = ToFollow->GetActorLocation();
		FVector DistanceVec = TargetLocation - AgentLocation;
		float Distance = DistanceVec.Size();

		if (OnlyWithinSight) {
			// Get angle to target
			float TargetAngle = FGenericPlatformMath::Acos(FVector::DotProduct(DistanceVec / Distance, Parent->GetActorForwardVector()));

			// Get trace to target
			FVector SocketLocation = AgentLocation;
			UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Parent->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
			if (Mesh && Mesh->DoesSocketExist("CameraSocket")) {
				SocketLocation = Mesh->GetSocketLocation("CameraSocket");
			}

			FVector StartVec = (TargetLocation - SocketLocation) * .1 + SocketLocation;
			FVector EndVec = (TargetLocation + FVector(0, 0, TargetHeight) - SocketLocation) * 2 + SocketLocation;

			FHitResult Hit = FHitResult();
			bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartVec, EndVec, ECollisionChannel::ECC_Visibility, FCollisionQueryParams());

			// Evaluate - if the actor is in our field of view and either the ray trace has intersected with the target or there is nothing between ourself and the target
			if (TargetAngle < FOVRadians && (Hit.Actor == ToFollow || Hit.Actor == nullptr))
				Reward = MaxScore * (MinDistance - Distance) / MinDistance;
			else
				Reward = 0;

		}
		else {
			Reward = MaxScore * (MinDistance - Distance) / MinDistance;
		}
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
