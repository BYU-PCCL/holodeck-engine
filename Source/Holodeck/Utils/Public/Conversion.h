#include "Holodeck.h"
#pragma once

const float UEUnitsPerMeter = 100.0;
const float UEUnitsPerMeterSquared = 10000;

enum ConvertType {UEToClient, ClientToUE, NoScale};

FVector ConvertLinearVector(FVector Vector, ConvertType Type);


FVector ConvertAngularVector(FVector Vector, ConvertType Type);

FRotator ConvertAngularVector(FRotator Rotator, ConvertType);


FVector ConvertTorque(FVector Vector, ConvertType Type);


