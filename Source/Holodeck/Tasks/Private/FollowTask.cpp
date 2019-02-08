#include "Holodeck.h"
#include "FollowTask.h"

// Set default values
void UFollowTask::InitializeSensor() {
	Super::InitializeSensor();
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
