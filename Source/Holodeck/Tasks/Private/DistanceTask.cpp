#include "Holodeck.h"
#include "DistanceTask.h"

void UDistanceTask::InitializeHolodeckComponent() {
	Super::InitializeHolodeckComponent();

	ComponentName = TASK_NAME;

	StartDistance = (GoalObject->GetComponentLocation() - Parent->GetComponentLocation()).Size();
	NextDistance = StartDistance - Interval;
	LastDistance = 1;
}

// Called every frame
void UDistanceTask::TickHolodeckComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (IsValid(Parent)) {
		if (UseDistanceReward)
			SetDistanceReward();
		else
			SetUnitReward();
	}

	Super::TickHolodeckComponent(DeltaTime, TickType, ThisTickFunction);
}

// Sets the reward if UseDistanceReward is false
void UDistanceTask::SetUnitReward() {
	float Distance = (GoalObject->GetComponentLocation() - Parent->GetComponentLocation()).Size();
	if (Distance < NextDistance) {
		Reward = 1;
		NextDistance -= Interval;
	} else {
		Reward = -1;
	}
	Terminal = Distance < GoalDistance;
}

// Sets the reward if UseDistanceReward is true
void UDistanceTask::SetDistanceReward() {
	float Distance = (GoalObject->GetComponentLocation() - Parent->GetComponentLocation()).Size() / StartDistance;
	Reward = LastDistance - Distance;
	LastDistance = Distance;
	Terminal = Distance < GoalDistance;
}
