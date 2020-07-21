// Fill out your copyright notice in the Description page of Project Settings.


#include "GolemBoss.h"
#include "HitBox/HitBoxMTP.h"

#include "Boss/CircleSkillRange.h"

#include "Skill/EarthQuakeSkill.h"

#define RAND_TERM ((rand() % 3) + 4)

AGolemBoss::AGolemBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolem_Anim_BP.DirtAStoneGolem_Anim_BP_C'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Boss/1Stage/Golem/DirtAStoneGolemBoss.DirtAStoneGolemBoss'"));

	GetCapsuleComponent()->SetRelativeScale3D(FVector(1, 1, 1));
	GetCapsuleComponent()->SetCapsuleRadius(75);

	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, 25));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->AnimClass = AnimClass.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMo(TEXT("AnimMontage'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolemBoss_Armature_Dead_Montage.DirtAStoneGolemBoss_Armature_Dead_Montage'"));
	DeadAnim = DeadMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolemBoss_Armature_Attack_Montage.DirtAStoneGolemBoss_Armature_Attack_Montage'"));
	AnimArray.Add(AttackMo.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToGroundMo(TEXT("AnimMontage'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolemBoss_Armature_AttackToGround_Montage.DirtAStoneGolemBoss_Armature_AttackToGround_Montage'"));
	AnimArray.Add(AttackToGroundMo.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GroundAttackMo(TEXT("AnimMontage'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolemBoss_Armature_GroundAttack_Montage.DirtAStoneGolemBoss_Armature_GroundAttack_Montage'"));
	AnimArray.Add(GroundAttackMo.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkyAttackMo(TEXT("AnimMontage'/Game/Boss/1Stage/Golem/Animation/DirtAStoneGolemBoss_Armature_SkyAttack_Montage.DirtAStoneGolemBoss_Armature_SkyAttack_Montage'"));
	AnimArray.Add(SkyAttackMo.Object);
}

void AGolemBoss::Attack()
{
	switch (AttackParam)
	{
	case 0:
		AttackFunc();
		break;
		
	case 1:
		AttackToGround();
		break;
	}
}

void AGolemBoss::AttackEnd()
{
	HitBox->SetCollisionEnable(ECollisionEnabled::NoCollision);
}

void AGolemBoss::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	HitBox = GetWorld()->SpawnActor<AHitBoxMTP>(AHitBoxMTP::StaticClass(),
		FTransform(
			GetActorRotation(),
			GetActorLocation(),
			FVector(HitBoxSize.X, HitBoxSize.Y, 1)));

	HitBox->SetDamage(1);
	HitBox->OwnerActor = this;

	GetWorldTimerManager().SetTimer(AttackTerm, this, &AGolemBoss::AttackTermFunc, 0.1, false, RAND_TERM);
}

void AGolemBoss::AttackTermFunc()
{
	End();
	AttackParam = -1;
	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetWorldTimerManager().SetTimer(AttackTerm, this, &AGolemBoss::CanAttackFunc, 0.1, false, RAND_TERM);
}

void AGolemBoss::AttackFunc()
{
	HitBox->SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 100));
	HitBox->SetActorRotation(GetActorRotation());
	HitBox->SetActorScale3D(FVector(50, 50, 1));

	HitBox->ClearHitPlayer();
	HitBox->SetCollisionEnable(ECollisionEnabled::QueryAndPhysics);
}

void AGolemBoss::AttackToGround()
{
	FVector Lo = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
		FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y, 0.5), FVector(2, 2, 2)))->SetLifeTime(0.9, 0.9, AEarthQuakeSkill::StaticClass(), 2);
}

void AGolemBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector2D::Distance(FVector2D(Player->GetActorLocation()), FVector2D(GetActorLocation())) <= 150 &&
		bSuddenAttack)
	{
		bSuddenAttack = false;
		AnimInstance->Montage_Play(AnimArray[0]);

		GetWorldTimerManager().SetTimer(SuddenAttackTimer, this, &AGolemBoss::InitSuddenAttack, 3, false, 3);
		return;
	}

	if (bCanAttack)
	{
		bCanAttack = false;

		AttackParam = (rand() % 1) + 1;

		switch (AttackParam)
		{
		case 1:
			AnimInstance->Montage_Play(AnimArray[1]);
			break;
		}
	}
}
