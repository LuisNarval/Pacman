// Fill out your copyright notice in the Description page of Project Settings.


#include "PelletSpawner.h"
#include "Engine/World.h"
#include "Engine/CurveTable.h"
#include "Engine/CurveTable.h"
#include "Engine/EngineTypes.h"
#include "CoreMinimal.h"

// Sets default values
APelletSpawner::APelletSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APelletSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Error, TEXT("THE SPAWNER STARTED!"));

	ReadStageCurveTable();
	PrintStageArray();
	//SpawnPelletsOnStage();
}

// Called every frame
void APelletSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	UWorld* World = GetWorld();

	if (World)
	{
		if (BP_MyActor_Class)
		{
			FVector SpawnLocation(0.f, 0.f, 0.f);
			FRotator SpawnRotation(0.f, 0.f, 0.f);
			AActor* SpawnedActor = World->SpawnActor<AActor>(BP_MyActor_Class, SpawnLocation, SpawnRotation);
		}
	}
}