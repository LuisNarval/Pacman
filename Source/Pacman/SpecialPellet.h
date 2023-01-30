// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpecialPellet.generated.h"

UCLASS()
class PACMAN_API ASpecialPellet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpecialPellet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};