#include "Holodeck.h"
#include "Conversion.h"

const bool USE_RHS = true;

FVector ConvertLinearVector(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeter;
	else if(Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeter;

	Vector.X *= ScaleFactor;
	Vector.Y *= ScaleFactor;
	Vector.Z *= ScaleFactor;

	if(USE_RHS)
		Vector.Y *= -1;

	return Vector;
}

FVector ConvertAngularVector(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeter;
	else if (Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeter;

	Vector.X *= ScaleFactor;
	Vector.Y *= ScaleFactor;
	Vector.Z *= ScaleFactor;

	if (USE_RHS) {
		Vector.X *= -1;
		Vector.Z *= -1;
	}

	return Vector;
}


FRotator ConvertAngularVector(FRotator Rotator, ConvertType Type) {
	return Rotator;
}

FVector ConvertTorque(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeterSquared;
	else if (Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeterSquared;

	Vector.X *= ScaleFactor;
	Vector.Y *= ScaleFactor;
	Vector.Z *= ScaleFactor;

	if (USE_RHS) {
		Vector.X *= -1;
		Vector.Z *= -1;
	}

	return Vector;
}


