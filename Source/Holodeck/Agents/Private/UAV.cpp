// Fill out your copyright notice in the Description page of Project Settings.
#include "Holodeck.h"
#include "UAV.h"
#include <math.h>

//All in radians. (rad/s, rad/s^2, etc.)
const float UAV_MASS = 3.856; //Kilograms
const float UAV_MU = 1;
const float UAV_MAX_ROLL = 6.5080;
const float UAV_MAX_PITCH = 5.087;
const float UAV_MAX_YAW_RATE = .8;
const float UAV_MAX_FORCE = 59.844;
const float UAV_TAU_UP_ROLL = 0.1904;
const float UAV_TAU_UP_PITCH = 0.1904;
const float UAV_TAU_UP_YAW_RATE = 0.04; // 1644;
const float UAV_TAU_UP_FORCE = 0.1644;
const float UAV_TAU_DOWN_ROLL = 0.1904;
const float UAV_TAU_DOWN_PITCH = 0.1904;
const float UAV_TAU_DOWN_YAW_RATE = 0.04;// 0.2164;
const float UAV_TAU_DOWN_FORCE = 0.2164;
const float UAV_ROLL_P = 25.0;
const float UAV_ROLL_I = 0.0;
const float UAV_ROLL_D = 8.0;
const float UAV_PITCH_P = 25.0;
const float UAV_PITCH_I = 0.0;
const float UAV_PITCH_D = 8.0;
const float UAV_YAW_P = 20.0;
const float UAV_YAW_I = 0.0;
const float UAV_YAW_D = 5.0;
const float UAV_ALT_P = 305.0;
const float UAV_ALT_I = 100.0;
const float UAV_ALT_D = 600.0;
const static int NUMBER_OF_PARAMETERS = 27;

enum ParameterIndices {
	TOTAL_PARAMETERS_INDEX = 0, //This is an index, not the actual count. 
	UAV_MASS_INDEX = 1,
	UAV_MU_INDEX = 2,
	UAV_MAX_ROLL_INDEX = 3,
	UAV_MAX_PITCH_INDEX = 4,
	UAV_MAX_YAW_RATE_INDEX = 5,
	UAV_MAX_FORCE_INDEX = 6,
	UAV_TAU_UP_ROLL_INDEX = 7,
	UAV_TAU_UP_PITCH_INDEX = 8,
	UAV_TAU_UP_YAW_RATE_INDEX = 9,
	UAV_TAU_UP_FORCE_INDEX = 10,
	UAV_TAU_DOWN_ROLL_INDEX = 11,
	UAV_TAU_DOWN_PITCH_INDEX = 12,
	UAV_TAU_DOWN_YAW_RATE_INDEX = 13,
	UAV_TAU_DOWN_FORCE_INDEX = 14,
	UAV_ROLL_P_INDEX = 15,
	UAV_ROLL_I_INDEX = 16,
	UAV_ROLL_D_INDEX = 17,
	UAV_PITCH_P_INDEX = 18,
	UAV_PITCH_I_INDEX = 19,
	UAV_PITCH_D_INDEX = 20,
	UAV_YAW_P_INDEX = 21,
	UAV_YAW_I_INDEX = 22,
	UAV_YAW_D_INDEX = 23,
	UAV_ALT_P_INDEX = 24,
	UAV_ALT_I_INDEX = 25,
	UAV_ALT_D_INDEX = 26
};

AUAV::AUAV() 
		: HyperparametersPointer(GetHyperparameters())
{
	RollController = SimplePID(HyperparametersPointer + UAV_ROLL_P_INDEX, HyperparametersPointer + UAV_ROLL_I_INDEX, HyperparametersPointer + UAV_ROLL_D_INDEX);
	PitchController = SimplePID(HyperparametersPointer + UAV_PITCH_P_INDEX, HyperparametersPointer + UAV_PITCH_I_INDEX, HyperparametersPointer + UAV_PITCH_D_INDEX);
	YawController = SimplePID(HyperparametersPointer + UAV_YAW_P_INDEX, HyperparametersPointer + UAV_YAW_I_INDEX, HyperparametersPointer + UAV_YAW_D_INDEX);
	AltitudeController = SimplePID(HyperparametersPointer + UAV_ALT_P_INDEX, HyperparametersPointer + UAV_ALT_I_INDEX, HyperparametersPointer + UAV_ALT_D_INDEX);

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the defualt controller
	AIControllerClass = LoadClass<AController>(NULL, TEXT("/Script/Holodeck.HolodeckUAVController"), NULL, LOAD_None, NULL);
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// TODO: the unreal unit conversion should be derived via global settings
	// TODO: the physics substep doesn't seem to have updated velocity/position estimates - we likely need to access the physx body directly (e.g. https://github.com/EpicGames/UnrealEngine/pull/585)
	// TODO: add changes seen in https://answers.unrealengine.com/questions/7459/question-is-120-the-engine-max-frame-rate.html
	// TODO: set deltaTick to 1/40th of a second
	SetActorEnableCollision(true);
	// OnCalculateCustomPhysics.BindUObject(this, &AUAV::SubstepTick);
}

void AUAV::BeginPlay() {
	Super::BeginPlay();
	RootMesh = Cast<UStaticMeshComponent>(RootComponent);
}

void AUAV::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//if (RootMesh) {
	//  RootMesh->GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	//}
	UpdateForcesAndMoments(DeltaTime);
	ApplyForces();
}

//void AUAV::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
//{
//  UpdateForcesAndMoments(DeltaTime);
//  ApplyForces();
//}

FVector AUAV::RotatorToEulerInZYX(FRotator Rotator) {
	FQuat Quaternion = Rotator.Quaternion();
	float Roll = atan2(2.0 * (Quaternion.W * Quaternion.X + Quaternion.Y * Quaternion.Z), 1.0 - 2.0 * (Quaternion.X * Quaternion.X + Quaternion.Y * Quaternion.Y));
	float Pitch = asin(2.0 * (Quaternion.W * Quaternion.Y - Quaternion.Z * Quaternion.X));
	float Yaw = atan2(2.0 * (Quaternion.W * Quaternion.Z + Quaternion.X * Quaternion.Y), 1.0 - 2.0 * (Quaternion.Y * Quaternion.Z + Quaternion.Z * Quaternion.Z));
	return FVector(Roll, Pitch, Yaw);
}

void AUAV::UpdateForcesAndMoments(float DeltaTime) {
	GEngine->GameUserSettings->SetScreenResolution(FIntPoint(10, 10)); // TODO - Why is this needed?
	//TODO(mitch): Get rid of/fix the unneccessary conversions. 
	// Get the current locations
	CurrentPositionX = UEUnitsToMeters(GetActorLocation().X);
	CurrentPositionY = UEUnitsToMeters(GetActorLocation().Y);
	CurrentPositionZ = UEUnitsToMeters(GetActorLocation().Z);

	// Get the rotator to get state and transform from world to local coordinates
	FRotator CurrentRotator = GetActorRotation();
	FVector EulerRotation = RotatorToEulerInZYX(CurrentRotator);  // Get these in local coords in (Z, Y, X) order - CurrentRotator.Euler() provides (X, Y, Z)
	FVector CurrentGlobalVelocity = GetVelocity();
	FVector LocalAngularVelocity = CurrentRotator.UnrotateVector(RootMesh->GetPhysicsAngularVelocity());

	CurrentRoll = EulerRotation.X;
	CurrentPitch = EulerRotation.Y;
	CurrentYaw = EulerRotation.Z;
	CurrentGlobalVelocityZ = UEUnitsToMeters(CurrentGlobalVelocity.Z);

	float CurrentRollRate = FMath::DegreesToRadians(LocalAngularVelocity.X);
	float CurrentPitchRate = FMath::DegreesToRadians(LocalAngularVelocity.Y);
	CurrentYawRate = FMath::DegreesToRadians(LocalAngularVelocity.Z);

	// Convert to proper coordinate frame (we want to respect Unreal's coordinate frame)
	CurrentRoll *= -1;
	CurrentPitch *= -1;
	CurrentRollRate *= -1;
	CurrentPitchRate *= -1;

	// Calculate the force and torques from the PID controller
	RollTorqueToApply = RollController.ComputePIDDirect(DesiredRoll, CurrentRoll, CurrentRollRate, DeltaTime);
	PitchTorqueToApply = PitchController.ComputePIDDirect(DesiredPitch, CurrentPitch, CurrentPitchRate, DeltaTime);
	YawTorqueToApply = YawController.ComputePID(DesiredYawRate, CurrentYawRate, DeltaTime);
	float HoverThrust = (RootMesh->GetMass() * -UEUnitsToMeters(GWorld->GetGravityZ())) / (cos(DesiredRoll) * cos(DesiredPitch));
	ThrustToApply = AltitudeController.ComputePIDDirect(DesiredAltitude, CurrentPositionZ, CurrentGlobalVelocityZ, DeltaTime) + HoverThrust;

	// Calculate first-order filter
	float TauRoll = (RollTorqueToApply > CurrentRollTorque) ? HyperparametersPointer[UAV_TAU_UP_ROLL_INDEX] : HyperparametersPointer[UAV_TAU_DOWN_ROLL_INDEX];
	float TauPitch = (PitchTorqueToApply > CurrentPitchTorque) ? HyperparametersPointer[UAV_TAU_UP_PITCH_INDEX] : HyperparametersPointer[UAV_TAU_DOWN_PITCH_INDEX];
	float TauYawRate = (YawTorqueToApply > CurrentYawTorque) ? HyperparametersPointer[UAV_TAU_UP_YAW_RATE_INDEX] : HyperparametersPointer[UAV_TAU_DOWN_YAW_RATE_INDEX];
	float TauThrust = (ThrustToApply > CurrentThrust) ? HyperparametersPointer[UAV_TAU_UP_FORCE_INDEX] : HyperparametersPointer[UAV_TAU_DOWN_FORCE_INDEX];
	float AlphaRoll = DeltaTime / (TauRoll + DeltaTime);
	float AlphaPitch = DeltaTime / (TauPitch + DeltaTime);
	float AlphaYaw = DeltaTime / (TauYawRate + DeltaTime);
	float AlphaThrust = DeltaTime / (TauThrust + DeltaTime);
	RollTorqueToApply = (1 - AlphaRoll) * RollTorqueToApply + AlphaRoll * RollTorqueToApply;
	PitchTorqueToApply = (1 - AlphaPitch) * PitchTorqueToApply + AlphaPitch * PitchTorqueToApply;
	YawTorqueToApply = (1 - AlphaYaw) * YawTorqueToApply + AlphaYaw * YawTorqueToApply;
	ThrustToApply = (1 - AlphaThrust) * ThrustToApply + AlphaThrust * ThrustToApply;

	// Calculate Air Resistance
	Wind = -HyperparametersPointer[UAV_MU_INDEX] * CurrentGlobalVelocity;

	// Apply the discrete first order filter
	RollTorqueToApply = FMath::Clamp(RollTorqueToApply, -HyperparametersPointer[UAV_MAX_ROLL_INDEX], HyperparametersPointer[UAV_MAX_ROLL_INDEX]);
	PitchTorqueToApply = FMath::Clamp(PitchTorqueToApply, -HyperparametersPointer[UAV_MAX_PITCH_INDEX], HyperparametersPointer[UAV_MAX_PITCH_INDEX]);
	YawTorqueToApply = FMath::Clamp(YawTorqueToApply, -HyperparametersPointer[UAV_MAX_YAW_RATE_INDEX], HyperparametersPointer[UAV_MAX_YAW_RATE_INDEX]);
	ThrustToApply = FMath::Clamp(ThrustToApply, -HyperparametersPointer[UAV_MAX_FORCE_INDEX], HyperparametersPointer[UAV_MAX_FORCE_INDEX]);
}

float AUAV::UEUnitsToMeters(float ValueInUnrealUnits) {
	return ValueInUnrealUnits / 100.0;
}

void AUAV::ApplyForces() {
	FVector LocalThrust = FVector(0, 0, ThrustToApply);
	FVector LocalTorque = FVector(-RollTorqueToApply, -PitchTorqueToApply, YawTorqueToApply); //Convert to proper coordinate frame
	LocalThrust *= 100;
	LocalTorque *= 10000;
	// Scale force for substepping
	//LocalThrust /= 16;
	//LocalTorque /= 16;
	//Wind /= 16
	// Apply torques and forces in global coordinates
	RootMesh->AddTorque(GetActorRotation().RotateVector(LocalTorque));
	RootMesh->AddForce(GetActorRotation().RotateVector(LocalThrust));
	RootMesh->AddForce(Wind);
	CurrentRollTorque = RollTorqueToApply;
	CurrentPitchTorque = PitchTorqueToApply;
	CurrentYawTorque = YawTorqueToApply;
	CurrentThrust = ThrustToApply;
}

const float* AUAV::GetDefaultHyperparameters() const {
	static const float DefaultHyperparameters[NUMBER_OF_PARAMETERS] = {
			NUMBER_OF_PARAMETERS,
			UAV_MASS,
			UAV_MU,
			UAV_MAX_ROLL,
			UAV_MAX_PITCH,
			UAV_MAX_YAW_RATE,
			UAV_MAX_FORCE,
			UAV_TAU_UP_ROLL,
			UAV_TAU_UP_PITCH,
			UAV_TAU_UP_YAW_RATE,
			UAV_TAU_UP_FORCE,
			UAV_TAU_DOWN_ROLL,
			UAV_TAU_DOWN_PITCH,
			UAV_TAU_DOWN_YAW_RATE,
			UAV_TAU_DOWN_FORCE,
			UAV_ROLL_P,
			UAV_ROLL_I,
			UAV_ROLL_D,
			UAV_PITCH_P,
			UAV_PITCH_I,
			UAV_PITCH_D,
			UAV_YAW_P,
			UAV_YAW_I,
			UAV_YAW_D,
			UAV_ALT_P,
			UAV_ALT_I,
			UAV_ALT_D,
		};
	return DefaultHyperparameters;
}

int AUAV::GetHyperparameterCount() const {
	return NUMBER_OF_PARAMETERS;
}

void AUAV::SetHyperparameterAddress(float* Input) {
	Super::SetHyperparameterAddress(Input);
	this->HyperparametersPointer = Input;
	InitializePIDControllers(); //Must give the PID Controllers the new addresses to point to.
}

void AUAV::InitializePIDControllers() {
	RollController.SetGains(HyperparametersPointer + UAV_ROLL_P_INDEX, HyperparametersPointer + UAV_ROLL_I_INDEX, HyperparametersPointer + UAV_ROLL_D_INDEX);
	PitchController.SetGains(HyperparametersPointer + UAV_PITCH_P_INDEX, HyperparametersPointer + UAV_PITCH_I_INDEX, HyperparametersPointer + UAV_PITCH_D_INDEX);
	YawController.SetGains(HyperparametersPointer + UAV_YAW_P_INDEX, HyperparametersPointer + UAV_YAW_I_INDEX, HyperparametersPointer + UAV_YAW_D_INDEX);
	AltitudeController.SetGains(HyperparametersPointer + UAV_ALT_P_INDEX, HyperparametersPointer + UAV_ALT_I_INDEX, HyperparametersPointer + UAV_ALT_D_INDEX);
}
