#include "Conversion.h"

FVector& ConvertLinearVector(FVector& Vector) {
	Vector.X *= UEUnitsPerMeter;
	Vector.Y *= -UEUnitsPerMeter; // Negative accounts for left handed coordinate system to right handed coordinate system change
	Vector.Z *= UEUnitsPerMeter;
	return Vector;
}

FVector& ConvertAngularVector(FVector& Vector) {
	Vector.X *= -UEUnitsPerMeter;
	Vector.Y *= UEUnitsPerMeter;
	Vector.Z *= -UEUnitsPerMeter;
	return Vector;
}

FVector& ConvertTorque(FVector& Vector) {
	Vector.X *= -UEUnitsPerMeterSquared;
	Vector.Y *= UEUnitsPerMeterSquared;
	Vector.Z *= -UEUnitsPerMeterSquared;
	return Vector;
}


