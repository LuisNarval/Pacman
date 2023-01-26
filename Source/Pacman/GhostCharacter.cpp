// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostCharacter.h"

#include "GhostsAbilitySystemComponent.h"
#include "GhostsAttributeSet.h"
#include "GhostsGameplayAbility.h"
#include <GameplayEffectTypes.h>

// Sets default values
AGhostCharacter::AGhostCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the Ability System Component for Pacman
	AbilitySystemComponent = CreateDefaultSubobject<UGhostsAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent-> SetIsReplicated(true);
	AbilitySystemComponent-> SetReplicationMode(EGameplayEffectReplicationMode::Full);

	//Create the Attributes for Pacman
	Attributes = CreateDefaultSubobject<UGhostsAttributeSet>("Attributes");
}

UAbilitySystemComponent* AGhostCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGhostCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGhostCharacter::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGhostsGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility,
				1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this
			));
		}
	}
}

void AGhostCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	//The server grants the default abilities to Pacman
	GiveAbilities();
}

void AGhostCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
}


// Called when the game starts or when spawned
void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGhostCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhostCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
