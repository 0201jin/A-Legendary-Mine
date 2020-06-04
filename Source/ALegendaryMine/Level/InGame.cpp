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
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	MyHUD->InitHUD();

	if (Mapgen != nullptr)
	{
		delete(Mapgen);
	}

	Mapgen = new MapGeneratorSys(this);

	CardSystem = GetWorld()->SpawnActor<ACardSystem>(
		ACardSystem::StaticClass(),
		FTransform(FRotator(0, 0, 0),
			FVector(0, 0, -500),
			FVector(1, 1, 1)));

	CardSystem->SpawnCard();

	UpdateCamera.Broadcast();
	
	MyHUD->CardSelectWidgetMode();

	Mapgen->MapGen(0);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(Mapgen->GetStartLo()));
}

void AInGame::SpawnMonster(int _Stage, int _Number) //명령어 추후 반드시 지울 것
{
	FMonsterDataTableRow MonsterData = MyGameInstance->MonsterData[_Stage][_Number];

	switch (MonsterData.AttackType)
	{
	case E_MonsterAttackType::M_BumpType:
		GetWorld()->SpawnActor<AMonsterActor>(
			ABumpTypeMonster::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y, 200),
				FVector(1, 1, 1)))
			->SetData(MonsterData);
		break;

	case E_MonsterAttackType::M_MeleeType:
		GetWorld()->SpawnActor<AMonsterActor>(
			AMeleeType::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y, 200),
				FVector(1, 1, 1)))
			->SetData(MonsterData);
		break;

	case E_MonsterAttackType::M_StandOffType:
		GetWorld()->SpawnActor<AMonsterActor>(
			AStandOffTypeMonster::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y, 200),
				FVector(1, 1, 1)))
			->SetData(MonsterData);
		break;
	}
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

void AInGame::ShuffleCard()
{
	CardSystem->RotBAll();
}

void AInGame::CameraToCharacter()
{
	UpdateCameraCharacter.Broadcast();
	MyHUD->InGameWidgetMode();
	CardSystem->DestroyCard();
}
