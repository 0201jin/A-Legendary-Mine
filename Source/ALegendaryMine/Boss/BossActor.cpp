// Fill out your copyright notice in the Description page of Project Settings.


#include "BossActor.h"
#include "Level/InGame.h"

// Sets default values
ABossActor::ABossActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossActor::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();
}

void ABossActor::PossessedBy(AController* NewController)
{
	AiController = Cast<AAIController>(NewController);

	Controller = AiController;

	if (GetController())
		UE_LOG(LogTemp, Log, TEXT("Monster : Possessed Controller!"));
}

void ABossActor::Dead()
{
}

void ABossActor::Damage(int _Damage, AActor* _ACKActor)
{
	UE_LOG(LogTemp, Log, TEXT("BossMonster : Damaged!"));

	Health -= _Damage;

	if (Health <= 0 && !bDead)
	{
		bDead = true;
		Dead();
		Cast<AInGame>(GetWorld()->GetLevelScriptActor())->GetMapgen()->DestroyBoss();
		SetActorTickEnabled(false);
	}
}

void ABossActor::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("BossMonster : Attack!"));
}

void ABossActor::Pause()
{
	AnimInstance->Montage_Pause();
}

void ABossActor::End()
{
	AnimInstance->Montage_Stop(0.1);
}
