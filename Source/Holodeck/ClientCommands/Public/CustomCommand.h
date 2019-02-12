// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientCommands/Public/Command.h"
#include "CustomCommand.generated.h"

/**
 * 
 */
UCLASS()
class HOLODECK_API UCustomCommand : public UCommand
{
	GENERATED_BODY()

public:
	//See UCommand for the documentation of this overridden function. 
	void Execute() override;
};
