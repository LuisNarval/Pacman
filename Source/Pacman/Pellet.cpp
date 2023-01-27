// Fill out your copyright notice in the Description page of Project Settings.

#include "Pellet.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APellet::APellet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PelletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitBox"));
	PelletMesh-> SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APellet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APellet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

