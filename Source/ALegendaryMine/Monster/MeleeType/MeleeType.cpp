// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeType.h"
#include "HitBox/HitBoxMTP.h"

// Sets default values
AMeleeType::AMeleeType()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMeleeType::Attack()
{
	HitBox->SetActorLocation(GetActorLocation() + (GetActorForwardVector() * AttackDistance));
	HitBox->SetActorRotation(GetActorRotation());
	HitBox->SetActorScale3D(FVector(HitBoxSize.X, HitBoxSize.Y, 1));

	HitBox->ClearHitPlayer();
	HitBox->SetCollisionEnable(ECollisionEnabled::QueryAndPhysics);

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AMeleeType::CanAttack, AttackSpeed, false, AttackSpeed);
}

void AMeleeType::EndAttack()
{
	HitBox->SetCollisionEnable(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AMeleeType::BeginPlay()
{
	Super::BeginPlay();

	HitBox = GetWorld()->SpawnActor<AHitBoxMTP>(AHitBoxMTP::StaticClass(),
		FTransform(
			GetActorRotation(),
			GetActorLocation(),
			FVector(HitBoxSize.X, HitBoxSize.Y, 1)));

	HitBox->SetDamage(1);
}

void AMeleeType::IsNotStun()
{
	float Distance = FVector::Distance(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance->Montage_IsPlaying(AttackAnimation))
	{
		if (Distance <= AttackDistance && bCanAttack)
		{
			AiController->MoveToLocation(GetActorLocation());
			AnimInstance->Montage_Play(AttackAnimation);
		}
		else
			AiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
}

// Called every frame
void AMeleeType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bStun)
		IsNotStun();
	else
		AiController->MoveToLocation(GetActorLocation());
}
