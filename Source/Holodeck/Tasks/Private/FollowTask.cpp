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

		if (JsonParsed->HasTypedField<EJson::String>("FollowSocket")) {
			FollowSocket = JsonParsed->GetStringField("FollowSocket");
		}

		if (JsonParsed->HasTypedField<EJson::Boolean>("OnlyWithinSight")) {
			OnlyWithinSight = JsonParsed->GetBoolField("OnlyWithinSight");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("FOVRadians")) {
			FOVRadians = JsonParsed->GetNumberField("FOVRadians");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("MinDistance")) {
			MinDistance = ConvertClientDistanceToUnreal(JsonParsed->GetNumberField("MinDistance"));
		}
	}
}

// Called every frame
void UFollowTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (ToFollow == nullptr && ToFollowTag != "")
		ToFollow = FindActorWithTag(ToFollowTag);

	if (ToFollow) {
		// Get location and distance
		FVector StartVec = Parent->GetActorLocation();
		UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Parent->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
		if (Mesh && Mesh->DoesSocketExist("CameraSocket")) {
			StartVec = Mesh->GetSocketLocation("CameraSocket");
		}

		FVector EndVec = ToFollow->GetActorLocation();
		Mesh = (UStaticMeshComponent*)ToFollow->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
		if (Mesh && Mesh->DoesSocketExist(FName(*FollowSocket))) {
			EndVec = Mesh->GetSocketLocation(FName(*FollowSocket));
		}

		FVector DistanceVec = EndVec - StartVec;
		float Distance = DistanceVec.Size();

		if (OnlyWithinSight) {
			// Get angle to target
			float TargetAngle = FGenericPlatformMath::Acos(FVector::DotProduct(DistanceVec / Distance, Parent->GetActorForwardVector()));

			// Get trace to target
			FCollisionQueryParams QueryParams = FCollisionQueryParams();
			QueryParams.AddIgnoredActor(Parent);
			FHitResult Hit = FHitResult();
			bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartVec, EndVec, ECollisionChannel::ECC_Visibility, QueryParams);

			// Evaluate - if the actor is in our field of view and either the ray trace has intersected with the target or there is nothing between ourself and the target
			if (TargetAngle < FOVRadians && (Hit.Actor == ToFollow || Hit.Actor == nullptr) && Distance < MinDistance) {
				Reward = MaxScore * (MinDistance - Distance) / MinDistance;
			} else {
				Reward = 0;
			}
		}
		else {
			if (Distance < MinDistance) {
				Reward = MaxScore * (MinDistance - Distance) / MinDistance;
			} else {
				Reward = 0;
			}
		}
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
