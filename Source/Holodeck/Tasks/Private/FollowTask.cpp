#include "Holodeck.h"
#include "FollowTask.h"

void UFollowTask::InitializeSensor() {
	Super::InitializeSensor();
}

// Called every frame
void UFollowTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	FVector AgentLocation = Parent->GetComponentLocation();
	FVector TargetLocation = ToFollow->GetComponentLocation();
	FVector DistanceVec = TargetLocation - AgentLocation;
	float Distance = DistanceVec.Size();

	if (OnlyWithinSight) {
		float TargetAngle = FGenericPlatformMath::Acos(FVector::DotProduct(DistanceVec / Distance, Parent->GetForwardVector()));
		FVector EndVec = (TargetLocation + FVector(0, 0, AgentHeight) - AgentLocation) * 2 + AgentLocation;
		FHitResult Hit = FHitResult();
		bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, AgentLocation, EndVec, ECollisionChannel::ECC_Visibility, FCollisionQueryParams());

		// If agent is facing target and in line of sight
		if (TargetAngle < FOVRadians && Hit.Actor == ToFollow->GetAttachmentRootActor())
			Reward = MaxScore * (MinDistance - Distance) / MinDistance;
		else
			Reward = 0;

	} else {
		Reward = MaxScore * (MinDistance - Distance) / MinDistance;
	}

	Super::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}
