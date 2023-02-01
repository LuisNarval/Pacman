// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanAttributeSet.h"
#include "Net/UnrealNetwork.h"


UPacmanAttributeSet::UPacmanAttributeSet() 
{

}

void UPacmanAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPacmanAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UPacmanAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPacmanAttributeSet, Health, OldHealth);
}

void UPacmanAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPacmanAttributeSet, Speed, OldSpeed);
}

void UPacmanAttributeSet::OnRep_Pellets(const FGameplayAttributeData& OldPellets)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPacmanAttributeSet, Pellets, OldPellets);
}

void UPacmanAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPacmanAttributeSet, MaxHealth, OldMaxHealth);
}

void UPacmanAttributeSet::OnRep_PelletsEated(const FGameplayAttributeData& OldPelletsEated)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPacmanAttributeSet, PelletsEated, OldPelletsEated);
}