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

void ABossActor::Damage(int _Damage, AActor* _ACKActor)
{
	Health -= _Damage;

	if (Health <= 0)
	{
		Destroy();
		Cast<AInGame>(GetWorld()->GetLevelScriptActor())->GetMapgen()->DestroyBoss();
	}
}