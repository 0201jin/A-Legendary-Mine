// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterActor.h"

// Sets default values
AMonsterActor::AMonsterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("CharacterMesh");

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); //값을 올리면 옆으로 흐르듯 이동
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->GroundFriction = 100.0f;
}

void AMonsterActor::SetData(FMonsterData _Data)
{
	GetMesh()->SetSkeletalMesh(_Data.Skeleton);
	GetMesh()->AnimClass = _Data.Animation->GetAnimBlueprintGeneratedClass();
	GetMesh()->SetRelativeLocation(FVector(0, 0, _Data.Z));

	GetCharacterMovement()->MaxWalkSpeed = _Data.Speed;

	GetCapsuleComponent()->SetCapsuleRadius(_Data.Radius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(_Data.Height);

	ProjectileMesh = _Data.ProjectileMesh;
	AttackSpeed = _Data.AttackSpeed;
	AttackDamage = _Data.AttackDamage;
	DropMoney = _Data.DropMoney;
	Health = _Data.Health;

	SpawnDefaultController();
}

// Called when the game starts or when spawned
void AMonsterActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterActor::PossessedBy(AController* NewController)
{
	AiController = Cast<AAIController>(NewController);

	Controller = AiController;

	if (GetController())
		UE_LOG(LogTemp, Log, TEXT("Monster : Possessed Controller!"));
}

void AMonsterActor::Damage(int _Damage)
{

}

