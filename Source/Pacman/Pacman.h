// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPacmanAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Punch,
	EatPellet, 
	TurboSpeed,
	EatSpecialPellet
};

UENUM(BlueprintType)
enum class EGhostsAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Boo
};