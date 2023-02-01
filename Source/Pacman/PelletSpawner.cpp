// Fill out your copyright notice in the Description page of Project Settings.

#include "PelletSpawner.h"
#include "Engine/World.h"
#include "Engine/CurveTable.h"
#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"

// Sets default values
APelletSpawner::APelletSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APelletSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	ReadStageCurveTable();
	SpawnPelletsOnStage();
}

void APelletSpawner::ReadStageCurveTable()
{
	TMap<FName, FRealCurve*> RowMap;

	if (StageCurveTable) 
	{
		RowMap = StageCurveTable->GetRowMap();
		
		TArray<FName> RowKeys;
		RowMap.GetKeys(RowKeys);

		int32 NumberOfRows = RowKeys.Num();
		StageArray.SetNum(NumberOfRows);

		for (FName Key : RowKeys)
		{
			int32 RowIndex = FCString::Atoi(*Key.ToString());

			FRealCurve* realCurve = RowMap[RowKeys[RowIndex]];

			int32 NumberOfColumns = realCurve->GetNumKeys();
			StageArray[RowIndex].SetNum(NumberOfColumns);

			for (auto It = realCurve->GetKeyHandleIterator(); It; ++It)
			{
				FKeyHandle KeyHandle = *It;
				float Time = realCurve->GetKeyTime(KeyHandle);
				float Value = realCurve->GetKeyValue(KeyHandle);

				int32 ColumnIndex = It.GetIndex();
				int32 StageValue = FMath::RoundToInt(Value);

				StageArray[RowIndex][ColumnIndex] = StageValue;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There's NOT an Stage Curve Table assigned to the PelletSpawner blueprint."));
	}
}

void APelletSpawner::PrintStageArray()
{
	UE_LOG(LogTemp, Error, TEXT(" ---------------------------------------------------------------------------") );
	UE_LOG(LogTemp, Error, TEXT(" ---------------------------------------------------------------------------") );
	UE_LOG(LogTemp, Error, TEXT(" ----------------------------- STAGE_ARRAY ---------------------------------") );
	UE_LOG(LogTemp, Error, TEXT(" ---------------------------------------------------------------------------") );
	UE_LOG(LogTemp, Error, TEXT(" ---------------------------------------------------------------------------") );

	for (int i = 0; i < StageArray.Num(); i++) 
	{
		UE_LOG(LogTemp, Error, TEXT(" ---------------------------------------------------------------------------") );
		UE_LOG(LogTemp, Error, TEXT(" ROW : %d"), i);

		for(int j = 0; j < StageArray[i].Num(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Value at (%d, %d) : %d"), i, j, StageArray[i][j]);
		}
	}
}

void APelletSpawner::SpawnPelletsOnStage()
{
	World = GetWorld();

	if (BP_Pellet)
	{
		for (int i = 0; i < StageArray.Num(); i++)
		{
			for (int j = 0; j < StageArray[i].Num(); j++)
			{
				if(StageArray[i][j]>0)
				{
					FVector SpawnLocation;
					SpawnLocation.X = XOffset + (i * XDistance);
					SpawnLocation.Y = YOffset + (j * YDistance);
					SpawnLocation.Z = ZOffset;

					SpawnPelletAt(SpawnLocation, StageArray[i][j]);
				}
			}
		}
	}	
}

void APelletSpawner::SpawnPelletAt(FVector SpawnLocation, int32 PelletType)
{
	if (World) 
	{
		if(PelletType == 1)
		{
			AActor* spawnedPellet = World->SpawnActor<AActor>(BP_Pellet, SpawnLocation, FRotator::ZeroRotator);
			PelletsToWin++;
		}
		if (PelletType == 2)
		{
			AActor* spawnedPellet = World->SpawnActor<AActor>(BP_SpecialPellet, SpawnLocation, FRotator::ZeroRotator);
			PelletsToWin++;
		}
	}
}