// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerBoss.h"

#include "Boss/CircleSkillRange.h"

#include "Skill/RootSkill.h"
#include "Skill/SkySkill.h"

#include "Monster/StandOffType/Projectile.h"

#include "FlowerBoss.h"

#define RAND_TERM ((rand() % 3) + 2)

// Sets default values
AFlowerBoss::AFlowerBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_Animation.FlowerBoss_Animation_C'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Boss/1Stage/Flower/FlowerBoss_1.FlowerBoss_1'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileStaticMesh(TEXT("StaticMesh'/Game/Monster/Monster_Projectile.Monster_Projectile'"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> MissileParticle(TEXT("NiagaraSystem'/Game/Boss/1Stage/Flower/Skill/SkyAttackP1_NG.SkyAttackP1_NG'"));

	MissileEffect = MissileParticle.Object;

	ProjectileMesh = ProjectileStaticMesh.Object;

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3, 3, 3));
	GetCapsuleComponent()->SetCapsuleRadius(7);

	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->AnimClass = AnimClass.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToRootMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Attack_To_Root_Montage.FlowerBoss_1_Armature_Attack_To_Root_Montage'"));
	AnimArray.Add(AttackToRootMo.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToSideMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Fire_To_Side_Montage.FlowerBoss_1_Armature_Fire_To_Side_Montage'"));
	AnimArray.Add(AttackToSideMo.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToSkyMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Fire_To_Sky_Montage.FlowerBoss_1_Armature_Fire_To_Sky_Montage'"));
	AnimArray.Add(AttackToSkyMo.Object);
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Dead_Montage.FlowerBoss_1_Armature_Dead_Montage'"));
	DeadAnim = DeadMo.Object;

}

// Called when the game starts or when spawned
void AFlowerBoss::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	
	GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM);
}

void AFlowerBoss::AttackToRoot()
{
	FVector Lo = GetActorLocation();
	Lo.Z = 0.5;

	float iAT = 0.9;

	for (int i = 1; i < 9; i++)
	{
		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X + (100 * i), Lo.Y, Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X - (100 * i), Lo.Y, Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y + (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y - (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());
		
	}

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AFlowerBoss::End, 0.1, false, iAT + 0.5);
	GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM + iAT);
}

void AFlowerBoss::AttackToRootP1()
{
	FVector Lo = GetActorLocation();
	Lo.Z = 0.5;

	float iAT = 0.9;

	for (int i = 1; i < 9; i++)
	{
		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X + (100 * i), Lo.Y + (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X - (100 * i), Lo.Y - (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X - (100 * i), Lo.Y + (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X + (100 * i), Lo.Y - (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(iAT, ARootSkill::StaticClass());

	}

	UE_LOG(LogTemp, Log, TEXT("Patten 1"));

	GetWorldTimerManager().SetTimer(AttackTimer, this, &AFlowerBoss::End, 0.1, false, iAT + 0.5);
	GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM + iAT);
}

void AFlowerBoss::AttackToSide()
{
	FRotator Rot = GetActorRotation();

	for (int i = 0; i < 10; i++)
	{
		GetWorld()->SpawnActor<AProjectile>(
			AProjectile::StaticClass(),
			FTransform(
				Rot,
				FVector(GetActorLocation().X, GetActorLocation().Y, 50),
				FVector(2, 2, 2)))->SetData(ProjectileMesh, 300);

		Rot.Yaw += (360 / 10);
	}
}

void AFlowerBoss::AttackToSky()
{
	FVector Lo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
		FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y, 0.5), FVector(1.5, 1.5, 1.5)))->SetLifeTime(1, 0.25, ASkySkill::StaticClass(), 2.5);
}

void AFlowerBoss::PlayATS()
{
	AnimInstance->Montage_Play(AnimArray[2]);
}

void AFlowerBoss::AttackTermFunc()
{
	End();
	SetActorRotation(FRotator(0, 0, 0));
	AttackParam = -1;
	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::CanAttackFunc, 0.1, false, RAND_TERM);
}

void AFlowerBoss::Dead()
{
	End();
	AttackParam = -1;
	GetWorldTimerManager().ClearTimer(AttackTimer);
	AnimInstance->Montage_Play(DeadAnim);
}

void AFlowerBoss::Attack()
{
	switch (AttackParam)
	{
	case 0:
		AttackToRoot();
		break;

	case 1:
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MissileEffect, FVector(GetActorLocation().X, GetActorLocation().Y, 400));
		AttackToSky();
		break;

	case 2:
		SetActorRotation(FRotator(0, rand() % 360, 0));
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AFlowerBoss::AttackToSide, 0.5, true, 0);
		GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM + 5);
		break;

	case 3:
		AttackToRootP1();
		break;
	}
}

// Called every frame
void AFlowerBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanAttack)
	{
		bCanAttack = false;

		AttackParam = rand() % 4;

		switch (AttackParam)
		{
		case 0:
			AnimInstance->Montage_Play(AnimArray[0]);
			break;

		case 1:
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AFlowerBoss::PlayATS, 0.9, true, 0);
			GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM + 10);
			break;

		case 2:
			AnimInstance->Montage_Play(AnimArray[1]);
			break;

		case 3:
			AnimInstance->Montage_Play(AnimArray[0]);
			break;
		}
	}
	else if (AttackParam == 2)
	{
		AddActorWorldRotation(FRotator(0, 50 * DeltaTime, 0));
	}
}
