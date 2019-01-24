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
		
		// Get Trace to target
		FVector EndVec = (TargetLocation + FVector(0, 0, TargetHeight) - AgentLocation) * 2 + AgentLocation;
		FHitResult Hit = FHitResult();
		bool TraceResult = Parent->ActorLineTraceSingle(Hit, AgentLocation, EndVec, ECollisionChannel::ECC_Visibility, FCollisionQueryParams());

		// If agent is facing target and in line of sight
		if (TargetAngle < FOVRadians && Hit.Actor == ToFollow)
			Reward = MaxScore * (MinDistance - Distance) / MinDistance;
		else
			Reward = 0;

	} else {
		Reward = MaxScore * (MinDistance - Distance) / MinDistance;
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	Super::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
