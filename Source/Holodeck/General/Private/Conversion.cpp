#include "Holodeck.h"
#include "Conversion.h"

FVector ConvertLinearVector(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeter;
	else if(Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeter;

	Vector.X *= ScaleFactor;
	Vector.Y *= -ScaleFactor; // Negative accounts for left handed coordinate system to right handed coordinate system change
	Vector.Z *= ScaleFactor;
	return Vector;
}

FVector ConvertAngularVector(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeter;
	else if (Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeter;

	Vector.X *= -ScaleFactor;
	Vector.Y *= ScaleFactor;
	Vector.Z *= -ScaleFactor;
	return Vector;
}


FRotator ConvertAngularVector(FRotator Rotator, ConvertType Type) {
	FVector Vec = Rotator.Vector();
	FRotator Rot = ConvertAngularVector(Vec, Type).Rotation();
	return Rot;
}

FVector ConvertTorque(FVector Vector, ConvertType Type) {

	float ScaleFactor = 1.0;
	if (Type == UEToClient)
		ScaleFactor /= UEUnitsPerMeterSquared;
	else if (Type == ClientToUE)
		ScaleFactor *= UEUnitsPerMeterSquared;

	Vector.X *= -ScaleFactor;
	Vector.Y *= ScaleFactor;
	Vector.Z *= -ScaleFactor;
	return Vector;
}


