// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "GhostCharacter.generated.h"

UCLASS()
class PACMAN_API AGhostCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
	class UGhostsAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGhostsAttributeSet* Attributes;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* BoxCollider;
	
public:

	/*Effect that initializes the default attributes for Pacman*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	/*Gameplay abilities for Pacman*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGhostsGameplayAbility>> DefaultAbilities;

private:
	UPROPERTY()
	bool IsVulnerable;

	FVector StartLocation;
	FRotator StartRotation;

public:
	// Sets default values for this character's properties
	AGhostCharacter();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:
	virtual void VulnerabilityChanged(const FOnAttributeChangeData& Data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	UFUNCTION()
	void OnHitBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void CallAbility(EGhostsAbilityInputID AbilityInputID);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void MakeBoo(AActor* Enemy);

};