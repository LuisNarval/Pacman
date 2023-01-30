// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostsAttributeSet.h"
#include "Net/UnrealNetwork.h"


UGhostsAttributeSet::UGhostsAttributeSet()
{

}

void UGhostsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGhostsAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UGhostsAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGhostsAttributeSet, Health, OldHealth);
}

void UGhostsAttributeSet::OnRep_BooForce(const FGameplayAttributeData& OldBooForce)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGhostsAttributeSet, BooForce, OldBooForce);
}

void UGhostsAttributeSet::OnRep_Vulnerability(const FGameplayAttributeData& OldVulnerability)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGhostsAttributeSet, Vulnerability, OldVulnerability);
}