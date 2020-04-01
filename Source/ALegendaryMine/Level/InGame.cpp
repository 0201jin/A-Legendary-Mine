// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame.h"
#include "DrawDebugHelpers.h"

AInGame::AInGame()
{
}

AInGame::~AInGame()
{
}

UMyGameInstance * AInGame::GetMyGameInstance()
{
	return MyGameInstance;
}

void AInGame::BeginPlay()
{
	Super::BeginPlay();

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (Mapgen != nullptr)
	{
		delete(Mapgen);
	}

	Mapgen = new MapGeneratorSys(this);

	Mapgen->MapGen(0);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(Mapgen->GetStartLo()));
}

void AInGame::GenerateMap()
{
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	FlushPersistentDebugLines(GetWorld());

	Mapgen->DeleteMap();

	Mapgen->MapGen(0);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(Mapgen->GetStartLo()));
}

void AInGame::UpDateNavMesh(FVector _Size, FVector _Location)
{
	UpdateNavMeshSize.Broadcast(_Size, _Location);
}
