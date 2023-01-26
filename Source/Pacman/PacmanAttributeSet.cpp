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