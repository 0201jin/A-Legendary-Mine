// Fill out your copyright notice in the Description page of Project Settings.


#include "BumpTypeMonster.h"
#include "Player/PlayerPawn.h"

// Sets default values
ABumpTypeMonster::ABumpTypeMonster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cast<UCapsuleComponent>(RootComponent)->OnComponentHit.AddDynamic(this, &ABumpTypeMonster::OnHit);
}

// Called when the game starts or when spawned
void ABumpTypeMonster::BeginPlay()
{
	Super::BeginPlay();

}

void ABumpTypeMonster::Attack()
{
	bCanAttack = true;
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void ABumpTypeMonster::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bCanAttack)
	{
		if (Cast<APlayerPawn>(OtherActor))
		{
			if (Cast<UCapsuleComponent>(OtherComponent))
			{
				Cast<APlayerPawn>(OtherActor)->Damage(1);
				GetWorldTimerManager().SetTimer(AttackTimer, this, &ABumpTypeMonster::Attack, AttackSpeed, false, AttackSpeed);
				bCanAttack = false;
			}
		}
	}
}

// Called every frame
void ABumpTypeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bStun)
		AiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	else
		AiController->MoveToLocation(GetActorLocation());
}