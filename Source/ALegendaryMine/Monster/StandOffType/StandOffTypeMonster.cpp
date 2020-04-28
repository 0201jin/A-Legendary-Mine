// Fill out your copyright notice in the Description page of Project Settings.

#include "StandOffTypeMonster.h"
#include "Projectile.h"

// Sets default values
AStandOffTypeMonster::AStandOffTypeMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AStandOffTypeMonster::Attack()
{
	AProjectile* ProjectileClass = GetWorld()->SpawnActor<AProjectile>(
		AProjectile::StaticClass(),
		FTransform(
			GetActorRotation(),
			FVector(GetActorLocation().X, GetActorLocation().Y, 50),
			FVector(2, 2, 2)));

	ProjectileClass->SetData(ProjectileMesh, ProjectileSpeed);
}

// Called when the game starts or when spawned
void AStandOffTypeMonster::BeginPlay()
{
	Super::BeginPlay();
}

void AStandOffTypeMonster::IsNotStun()
{
	float Distance = FVector::Distance(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance->Montage_IsPlaying(AttackAnimation))
	{
		if (Distance <= AttackDistance)
		{
			FRotator DiRo = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation()).Rotation();
			DiRo.Roll = 0;

			SetActorRotation(DiRo);

			AiController->MoveToLocation(GetActorLocation());
			AnimInstance->Montage_Play(AttackAnimation);
		}
		else
			AiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
}

// Called every frame
void AStandOffTypeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bStun)
		IsNotStun();
	else
		AiController->MoveToLocation(GetActorLocation());
}