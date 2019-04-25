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
			FString ActorName = JsonParsed->GetStringField("ToFollow");
			for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->GetName() == ActorName) {
					ToFollow = *ActorItr;
					break;
				}
			}
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

		// Evaluate
		if (TargetAngle < FOVRadians && Hit.Actor == ToFollow)
			Reward = MaxScore * (MinDistance - Distance) / MinDistance;
		else
			Reward = 0;

	} else {
		Reward = MaxScore * (MinDistance - Distance) / MinDistance;
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
