// Fill out your copyright notice in the Description page of Project Settings.

#include "GhostCharacter.h"

#include "GhostsAbilitySystemComponent.h"
#include "GhostsAttributeSet.h"
#include "GhostsGameplayAbility.h"
#include <GameplayEffectTypes.h>

#include "Components/BoxComponent.h"
#include "PacmanCharacter.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "AbilitySystemComponent.h"



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

	//BoxColliders
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Box"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGhostCharacter::OnHitBoxOverlap);
}

UAbilitySystemComponent* AGhostCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGhostCharacter::InitializeAttributes()
{
	if (HasAuthority()) 
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Attributes->GetVulnerabilityAttribute()).AddUObject(this, &AGhostCharacter::VulnerabilityChanged);
	}

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

void AGhostCharacter::VulnerabilityChanged(const FOnAttributeChangeData& Data)
{
	if(Data.NewValue > 0)
	{
		IsVulnerable = true;
	}
	else
	{
		IsVulnerable = false;
	}
}

// Called when the game starts or when spawned
void AGhostCharacter::BeginPlay()
{
	Super::BeginPlay();
	IsVulnerable = false;
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
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

void AGhostCharacter::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTemp, Warning, TEXT("Collision something !!"));

	if (APacmanCharacter* PacmanCharacter = Cast<APacmanCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Error, TEXT("Collision with PACMAN !!"));

		if (IsVulnerable)
		{
			UE_LOG(LogTemp, Error, TEXT("Vulnerable!!"));
			ReturnToOrigin();
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("UnVulnerable!!"));
			MakeBoo(OtherActor);
		}
	}
}

void AGhostCharacter::ReturnToOrigin() 
{
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
}

void AGhostCharacter::MakeBoo(AActor* Enemy)
{
	CallAbility(EGhostsAbilityInputID::Boo);

	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag("Ghost.Boo");

	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = Enemy;
	EventData.EventMagnitude = 1.0f;

	AbilitySystemComponent->HandleGameplayEvent(EventTag, &EventData);
}

void AGhostCharacter::CallAbility(EGhostsAbilityInputID AbilityInputID)
{
	int32 InputID = (int32)AbilityInputID;

	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputID);
	FGameplayAbilitySpecHandle AbilitySpecHandle = Spec->Handle;

	AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle, true);
}