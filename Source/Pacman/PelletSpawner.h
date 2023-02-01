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
	class UBlueprintGeneratedClass* BP_Pellet;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	class UBlueprintGeneratedClass* BP_SpecialPellet;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	class UCurveTable* StageCurveTable;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float XOffset = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float YOffset = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float ZOffset = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float XDistance = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	float YDistance = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawning")
	float PelletsToWin = 0;

private:
	TArray<TArray<int32>> StageArray;
	UWorld* World;

public:	
	// Sets default values for this actor's properties
	APelletSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ReadStageCurveTable();

	UFUNCTION()
	void PrintStageArray();

	UFUNCTION()
	void SpawnPelletsOnStage();

	UFUNCTION()
	void SpawnPelletAt(FVector SpawnLocation, int32 PelletType);
};