#include "Holodeck.h"
#include "UavControlSchemeTargetRollPitch.h"

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

UUavControlSchemeTargetRollPitch::UUavControlSchemeTargetRollPitch(const FObjectInitializer& ObjectInitializer) :
		Super(ObjectInitializer),
		RollController(UAV_ROLL_P, UAV_ROLL_I, UAV_ROLL_D),
		PitchController(UAV_PITCH_P, UAV_PITCH_I, UAV_PITCH_D),
		YawController(UAV_YAW_P, UAV_YAW_I, UAV_YAW_D),
		AltitudeController(UAV_ALT_P, UAV_ALT_I, UAV_ALT_D) {}

void UUavControlSchemeTargetRollPitch::Execute(void* const CommandArray, void* const InputCommand, float DeltaSeconds) {
	if (Uav == nullptr) {
		Uav = static_cast<AUav*>(UavController->GetPawn());
		if (Uav == nullptr) {
			UE_LOG(LogHolodeck, Error, TEXT("UUAVControlSchemeTargetRollPitch couldn't get Uav reference"));
			return;
		}
	}

	float* CommandArrayFloat = static_cast<float*>(CommandArray);
	float* InputCommandFloat = static_cast<float*>(InputCommand);

	float DesiredRoll = InputCommandFloat[0];
	float DesiredPitch = InputCommandFloat[1];
	float DesiredYawRate = InputCommandFloat[2];
	float DesiredAltitude = InputCommandFloat[3];

	// Update forces and movements function
	float CurrentPositionZ = UEUnitsToMeters(Uav->GetActorLocation().Z);

	// Get the rotator to get state and transform from world to local coordinates
	FRotator CurrentRotator = Uav->GetActorRotation();
	FVector EulerRotation = RotatorToEulerInZYX(CurrentRotator);  // Get these in local coords in (Z, Y, X) order - CurrentRotator.Euler() provides (X, Y, Z)
	FVector CurrentGlobalVelocity = Uav->GetVelocity();
	FVector LocalAngularVelocity = CurrentRotator.UnrotateVector(Uav->RootMesh->GetPhysicsAngularVelocity());

	float CurrentRoll = EulerRotation.X;
	float CurrentPitch = EulerRotation.Y;
	float CurrentGlobalVelocityZ = UEUnitsToMeters(CurrentGlobalVelocity.Z);

	float CurrentRollRate = FMath::DegreesToRadians(LocalAngularVelocity.X);
	float CurrentPitchRate = FMath::DegreesToRadians(LocalAngularVelocity.Y);
	float CurrentYawRate = FMath::DegreesToRadians(LocalAngularVelocity.Z);

	// Convert to proper coordinate frame (we want to respect Unreal's coordinate frame)
	CurrentRoll *= -1;
	CurrentPitch *= -1;
	CurrentRollRate *= -1;
	CurrentPitchRate *= -1;

	// Calculate the force and torques from the PID controller
	float RollTorqueToApply = RollController.ComputePIDDirect(DesiredRoll, CurrentRoll, CurrentRollRate, DeltaSeconds);
	float PitchTorqueToApply = PitchController.ComputePIDDirect(DesiredPitch, CurrentPitch, CurrentPitchRate, DeltaSeconds);
	float YawTorqueToApply = YawController.ComputePID(DesiredYawRate, CurrentYawRate, DeltaSeconds);
	float HoverThrust = (Uav->RootMesh->GetMass() * -UEUnitsToMeters(GWorld->GetGravityZ())) / (cos(DesiredRoll) * cos(DesiredPitch));
	float ThrustToApply = AltitudeController.ComputePIDDirect(DesiredAltitude, CurrentPositionZ, CurrentGlobalVelocityZ, DeltaSeconds) + HoverThrust;

	// Calculate first-order filter
	float TauRoll = (RollTorqueToApply > CommandArrayFloat[0]) ? UAV_TAU_UP_ROLL : UAV_TAU_DOWN_ROLL;
	float TauPitch = (PitchTorqueToApply > CommandArrayFloat[1]) ? UAV_TAU_UP_PITCH : UAV_TAU_DOWN_PITCH;
	float TauYawRate = (YawTorqueToApply > CommandArrayFloat[2]) ? UAV_TAU_UP_YAW_RATE : UAV_TAU_DOWN_YAW_RATE;
	float TauThrust = (ThrustToApply > CommandArrayFloat[3]) ? UAV_TAU_UP_FORCE : UAV_TAU_DOWN_FORCE;
	float AlphaRoll = DeltaSeconds / (TauRoll + DeltaSeconds);
	float AlphaPitch = DeltaSeconds / (TauPitch + DeltaSeconds);
	float AlphaYaw = DeltaSeconds / (TauYawRate + DeltaSeconds);
	float AlphaThrust = DeltaSeconds / (TauThrust + DeltaSeconds);

	CommandArrayFloat[0] = (1 - AlphaRoll) * RollTorqueToApply + AlphaRoll * RollTorqueToApply;
	CommandArrayFloat[1] = (1 - AlphaPitch) * PitchTorqueToApply + AlphaPitch * PitchTorqueToApply;
	CommandArrayFloat[2] = (1 - AlphaYaw) * YawTorqueToApply + AlphaYaw * YawTorqueToApply;
	CommandArrayFloat[3] = (1 - AlphaThrust) * ThrustToApply + AlphaThrust * ThrustToApply;
}

FVector UUavControlSchemeTargetRollPitch::RotatorToEulerInZYX(const FRotator& Rotator) const {
	FQuat Quaternion = Rotator.Quaternion();
	float Roll = atan2(2.0 * (Quaternion.W * Quaternion.X + Quaternion.Y * Quaternion.Z), 1.0 - 2.0 * (Quaternion.X * Quaternion.X + Quaternion.Y * Quaternion.Y));
	float Pitch = asin(2.0 * (Quaternion.W * Quaternion.Y - Quaternion.Z * Quaternion.X));
	float Yaw = atan2(2.0 * (Quaternion.W * Quaternion.Z + Quaternion.X * Quaternion.Y), 1.0 - 2.0 * (Quaternion.Y * Quaternion.Z + Quaternion.Z * Quaternion.Z));
	return FVector(Roll, Pitch, Yaw);
}

float UUavControlSchemeTargetRollPitch::UEUnitsToMeters(float ValueInUnrealUnits) const {
	return ValueInUnrealUnits / UEUnitsPerMeter;
}
