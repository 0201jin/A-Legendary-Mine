// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowerBoss.h"

#include "Boss/CircleSkillRange.h"
#include "Skill/RootSkill.h"

#define RAND_TERM ((rand() % 3) + 2)

// Sets default values
AFlowerBoss::AFlowerBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_Animation.FlowerBoss_Animation_C'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/Boss/1Stage/Flower/FlowerBoss_1.FlowerBoss_1'"));

	GetCapsuleComponent()->SetRelativeScale3D(FVector(3, 3, 3));
	GetCapsuleComponent()->SetCapsuleRadius(7);

	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->AnimClass = AnimClass.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToRootMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Attack_To_Root_Montage.FlowerBoss_1_Armature_Attack_To_Root_Montage'"));
	AttackToRootAnim = AttackToRootMo.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToSideMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Fire_To_Side_Montage.FlowerBoss_1_Armature_Fire_To_Side_Montage'"));
	AttackToSideAnim = AttackToSideMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackToSkyMo(TEXT("AnimMontage'/Game/Boss/1Stage/Flower/Animation/FlowerBoss_1_Armature_Fire_To_Sky_Montage.FlowerBoss_1_Armature_Fire_To_Sky_Montage'"));
	AttackToSkyAnim = AttackToSkyMo.Object;
	
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

	for (int i = 1; i < 9; i++)
	{
		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X + (100 * i), Lo.Y, Lo.Z), FVector(1, 1, 1)))->SetLifeTime(3, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X - (100 * i), Lo.Y, Lo.Z), FVector(1, 1, 1)))->SetLifeTime(3, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y + (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(3, ARootSkill::StaticClass());

		GetWorld()->SpawnActor<ACircleSkillRange>(ACircleSkillRange::StaticClass(),
			FTransform(FRotator(0, 0, 0), FVector(Lo.X, Lo.Y - (100 * i), Lo.Z), FVector(1, 1, 1)))->SetLifeTime(3, ARootSkill::StaticClass());
		
	}

	GetWorldTimerManager().SetTimer(AttackTerm, this, &AFlowerBoss::AttackTermFunc, 0.1, false, RAND_TERM + 3);
}

void AFlowerBoss::AttackToSide()
{
}

void AFlowerBoss::AttackToSky()
{
}

// Called every frame
void AFlowerBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanAttack)
	{
		UE_LOG(LogTemp, Log, TEXT("Attack Sibal"));
		bCanAttack = false;

		AttackToRoot();
	}
}
