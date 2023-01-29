// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PelletSpawner.generated.h"

UCLASS()
class PACMAN_API APelletSpawner : public AActor
{
	GENERATED_BODY()

public:
UPROPERTY(EditDefaultsOnly, Category = "Spawning")
class UBlueprintGeneratedClass* BP_MyActor_Class;

UPROPERTY(EditDefaultsOnly, Category = "Spawning")
class UCurveTable* StageCurveTable;

private:
TArray<TArray<int32>> StageArray;

public:	
	// Sets default values for this actor's properties
	APelletSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void ReadStageCurveTable();

	UFUNCTION()
	void PrintStageArray();

	UFUNCTION()
	void SpawnPelletsOnStage();

};