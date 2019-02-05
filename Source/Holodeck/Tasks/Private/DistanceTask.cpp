#include "Holodeck.h"
#include "DistanceTask.h"

// Set default values
void UDistanceTask::InitializeSensor() {
	Super::InitializeSensor();

	StartDistance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size();
	NextDistance = StartDistance - Interval;
	LastDistance = 1;
}

// Called every frame
void UDistanceTask::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (IsValid(Parent)) {
		if (UseDistanceReward)
			SetDistanceReward();
		else
			SetUnitReward();
	}

	// Call TaskSensor's Tick to store Reward and Terminal
	UTaskSensor::TickSensorComponent(DeltaTime, TickType, ThisTickFunction);
}

// Sets the reward if UseDistanceReward is false
void UDistanceTask::SetUnitReward() {
	float Distance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size();
	if (Distance < NextDistance) {
		Reward = 1;
		NextDistance = Distance - Interval;
	} else {
		Reward = -1;
	}
	Terminal = Distance < GoalDistance;
}

// Sets the reward if UseDistanceReward is true
void UDistanceTask::SetDistanceReward() {
	float Distance = (GoalObject->GetActorLocation() - Parent->GetActorLocation()).Size() / StartDistance;
	Reward = LastDistance - Distance;
	LastDistance = Distance;
	Terminal = Distance < GoalDistance;
}
