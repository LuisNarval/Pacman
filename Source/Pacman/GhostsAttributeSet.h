// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GhostsAttributeSet.generated.h"


//Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class PACMAN_API UGhostsAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UGhostsAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	
	ATTRIBUTE_ACCESSORS(UGhostsAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);


	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BooForce)
	FGameplayAttributeData BooForce;

	ATTRIBUTE_ACCESSORS(UGhostsAttributeSet, BooForce);

	UFUNCTION()
	virtual void OnRep_BooForce(const FGameplayAttributeData& OldBooForce);

	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Vulnerability)
	FGameplayAttributeData Vulnerability;

	ATTRIBUTE_ACCESSORS(UGhostsAttributeSet, Vulnerability);

	UFUNCTION()
	virtual void OnRep_Vulnerability(const FGameplayAttributeData& OldVulnerability);
};