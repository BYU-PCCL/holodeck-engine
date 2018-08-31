// Fill out your copyright notice in the Description page of Project Settings.
#include "Holodeck.h"
#include "Uav.h"

//All in radians. (rad/s, rad/s^2, etc.)
const float UAV_MASS = 3.856; //Kilograms
const float UAV_MU = 1;
const float UAV_MAX_ROLL = 6.5080;
const float UAV_MAX_PITCH = 5.087;
const float UAV_MAX_YAW_RATE = .8;
const float UAV_MAX_FORCE = 59.844;
const static int NUMBER_OF_PARAMETERS = 7;

enum ParameterIndices {
	TOTAL_PARAMETERS_INDEX = 0, //This is an index, not the actual count. 
	UAV_MASS_INDEX = 1,
	UAV_MU_INDEX = 2,
	UAV_MAX_ROLL_INDEX = 3,
	UAV_MAX_PITCH_INDEX = 4,
	UAV_MAX_YAW_RATE_INDEX = 5,
	UAV_MAX_FORCE_INDEX = 6,
};

AUav::AUav() : HyperparametersPointer(GetHyperparameters()) {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.UavController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// TODO: the unreal unit conversion should be derived via global settings
	// TODO: the physics substep doesn't seem to have updated velocity/position estimates - we likely need to access the physx body directly (e.g. https://github.com/EpicGames/UnrealEngine/pull/585)
	// TODO: add changes seen in https://answers.unrealengine.com/questions/7459/question-is-120-the-engine-max-frame-rate.html
	// TODO: set deltaTick to 1/40th of a second
	SetActorEnableCollision(true);
	// OnCalculateCustomPhysics.BindUObject(this, &AUav::SubstepTick);
}

void AUav::BeginPlay() {
	Super::BeginPlay();
	RootMesh = Cast<UStaticMeshComponent>(RootComponent);
}

void AUav::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//if (RootMesh) {
	//  RootMesh->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	//}
	ApplyForces();
}

//void AUav::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
//{
//  UpdateForcesAndMoments(DeltaTime);
//  ApplyForces();
//}

void AUav::ApplyForces() {
	// Calculate Air Resistance
	FVector Wind = -HyperparametersPointer[UAV_MU_INDEX] * GetVelocity();

	float RollTorqueToApply = FMath::Clamp(GetRollTorqueToApply(), -HyperparametersPointer[UAV_MAX_ROLL_INDEX], HyperparametersPointer[UAV_MAX_ROLL_INDEX]);
	float PitchTorqueToApply = FMath::Clamp(GetPitchTorqueToApply(), -HyperparametersPointer[UAV_MAX_PITCH_INDEX], HyperparametersPointer[UAV_MAX_PITCH_INDEX]);
	float YawTorqueToApply = FMath::Clamp(GetYawTorqueToApply(), -HyperparametersPointer[UAV_MAX_YAW_RATE_INDEX], HyperparametersPointer[UAV_MAX_YAW_RATE_INDEX]);
	float ThrustToApply = FMath::Clamp(GetThrustToApply(), -HyperparametersPointer[UAV_MAX_FORCE_INDEX], HyperparametersPointer[UAV_MAX_FORCE_INDEX]);

	FVector LocalThrust = FVector(0, 0, ThrustToApply);
	FVector LocalTorque = FVector(RollTorqueToApply, PitchTorqueToApply, YawTorqueToApply);
	LocalThrust *= 100;
	LocalTorque *= 10000;
	// Scale force for substepping
	//LocalThrust /= 16;
	// LocalTorque /= 16;
	// Wind /= 16
	// Apply torques and forces in global coordinates
	RootMesh->AddTorque(GetActorRotation().RotateVector(LocalTorque));
	RootMesh->AddForce(GetActorRotation().RotateVector(LocalThrust));
	RootMesh->AddForce(Wind);

	//CurrentRollTorque = RollTorqueToApply;
	//CurrentPitchTorque = PitchTorqueToApply;
	//CurrentYawTorque = YawTorqueToApply;
	//CurrentThrust = ThrustToApply;
}

const float* AUav::GetDefaultHyperparameters() const {
	static const float DefaultHyperparameters[NUMBER_OF_PARAMETERS] = {
			NUMBER_OF_PARAMETERS,
			UAV_MASS,
			UAV_MU,
			UAV_MAX_ROLL,
			UAV_MAX_PITCH,
			UAV_MAX_YAW_RATE,
			UAV_MAX_FORCE
		};
	return DefaultHyperparameters;
}

int AUav::GetHyperparameterCount() const {
	return NUMBER_OF_PARAMETERS;
}

void AUav::SetHyperparameterAddress(float* Input) {
	Super::SetHyperparameterAddress(Input);
	this->HyperparametersPointer = Input;
	// InitializePIDControllers(); //Must give the PID Controllers the new addresses to point to.
}
