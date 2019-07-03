// MIT License (c) 2019 BYU PCCL see LICENSE file

#pragma once

#include "CoreMinimal.h"
#include "HolodeckCore/Public/HolodeckControlScheme.h"
#include "JointMaxTorqueControlScheme.h"
#include "HandAgent.h"

#include "HandAgentMaxTorqueFloat.generated.h"

/**
 * UHandAgentMaxTorqueFloat - like UAndroidControlSchemeMaxTorque, but the first three elements in the
 * command array control the Hand agent's movement in x, y, and z planes so it can float around.
 * It processes the first three elements, and then forwards the rest of the array along to a UAndroidControlSchemeMaxTorque
 * since it behaves identically.
 */
UCLASS()
class HOLODECK_API UHandAgentMaxTorqueFloat : public UHolodeckControlScheme
{
	GENERATED_BODY()

public:
	UHandAgentMaxTorqueFloat(const FObjectInitializer& ObjectInitializer) {};

	void Execute(void* const CommandArray, void* const InputCommand, float DeltaSeconds) override;

	unsigned int GetControlSchemeSizeInBytes() const override;

	void SetTorqueControlScheme(UJointMaxTorqueControlScheme* Scheme);

private:
	UJointMaxTorqueControlScheme *TorqueControlScheme;

};
