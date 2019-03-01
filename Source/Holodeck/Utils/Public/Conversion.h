#include "Holodeck.h"
#pragma once

const float UEUnitsPerMeter = 100.0;
const float UEUnitsPerMeterSquared = 10000;

enum ConvertType {UEToClient, ClientToUE, NoScale};

FVector& ConvertLinearVector(FVector& Vector, ConvertType Type = UEToClient);


FVector& ConvertAngularVector(FVector& Vector, ConvertType Type = UEToClient);


FVector& ConvertTorque(FVector& Vector, ConvertType Type = UEToClient);


