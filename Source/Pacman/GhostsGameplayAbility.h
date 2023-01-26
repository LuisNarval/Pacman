// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Pacman.h"
#include "GhostsGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API UGhostsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGhostsGameplayAbility();

	//Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGhostsAbilityInputID AbilityInputID = EGhostsAbilityInputID::None;

};