#include "Holodeck.h"
#include "Android.h"
#include "TaskHelper.h"


FVector GetActorSocketLocation(AActor* actor, FString socket_name) {
	FVector StartVec = actor->GetActorLocation();
	UStaticMeshComponent* Mesh = (UStaticMeshComponent*)actor->GetComponentByClass(TSubclassOf<UStaticMeshComponent>());
	if (Mesh && Mesh->DoesSocketExist(FName(*socket_name))) {
		StartVec = Mesh->GetSocketLocation(FName(*socket_name));
	}
	return StartVec;
}


bool IsInSight(AActor* seeing_actor, AActor* target_actor, FVector& start, FVector& end, float FOVRad, FVector& DistanceVec, float Distance) {

	FVector ForwardVector = seeing_actor->GetActorForwardVector();
	/*if ((dynamic_cast<AAndroid*> (seeing_actor)) != NULL) {
		float x = ForwardVector.X;
		ForwardVector.X = ForwardVector.Y;
		ForwardVector.Y = x;
	}*/

	// Get angle to target
	float TargetAngle = FGenericPlatformMath::Acos(FVector::DotProduct(DistanceVec / Distance, ForwardVector));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Angle %f"), TargetAngle));
	// Get trace to target
	FCollisionQueryParams QueryParams = FCollisionQueryParams();
	QueryParams.AddIgnoredActor(seeing_actor);
	FHitResult Hit = FHitResult();
	DrawDebugLine(seeing_actor->GetWorld(), start, start, FColor(0, 0, 255), false, 0.1, 0);
	bool TraceResult = seeing_actor->GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECollisionChannel::ECC_Visibility, QueryParams);

	// Evaluate - if the actor is in our field of view and either the ray trace has intersected with the target or there is nothing between ourself and the target
	return TargetAngle < FOVRad && (Hit.Actor == target_actor || Hit.Actor == nullptr);
}