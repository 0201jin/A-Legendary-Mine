// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterActor.h"
#include "Level/InGame.h"

// Sets default values
AMonsterActor::AMonsterActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("CharacterMesh");

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); //값을 올리면 옆으로 흐르듯 이동
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->GroundFriction = 100.0f;

	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
}

AMonsterActor::~AMonsterActor()
{
	if (AiController != nullptr)
		AiController->Destroy();
}

void AMonsterActor::SetData(FMonsterDataTableRow _Data)
{
	GetMesh()->SetSkeletalMesh(_Data.Skeleton);
	GetMesh()->SetAnimClass(_Data.Animation->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeLocation(FVector(0, 0, _Data.Z));
	GetMesh()->SetRelativeScale3D(_Data.Size);

	GetCharacterMovement()->MaxWalkSpeed = _Data.Speed;

	GetCapsuleComponent()->SetCapsuleRadius(_Data.Radius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(_Data.Height);

	ProjectileMesh = _Data.ProjectileMesh;
	AttackSpeed = _Data.AttackSpeed;
	AttackDistance = _Data.AttackDistance;
	DropMoney = _Data.DropMoney;
	Health = _Data.Health;
	AttackAnimation = _Data.AttackAnimation;
	HitBoxSize = _Data.HitBoxSize;

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

void AMonsterActor::NuckBack()
{
	AddActorLocalOffset(DiRo.Vector().ForwardVector * -5, true);
}

void AMonsterActor::Damage(int _Damage, AActor* _ACKActor)
{
	bStun = true;

	GetWorldTimerManager().SetTimer(StunTimer, this, &AMonsterActor::StunEnd, 0.5f, false, 0.3f);
	GetWorldTimerManager().SetTimer(NuckBackEndTimer, this, &AMonsterActor::NuckBackEnd, 0.5f, false, 0.1f);
	GetWorldTimerManager().SetTimer(NuckBackTimer, this, &AMonsterActor::NuckBack, 0.01f, true, 0.0f);

	//AddActorLocalOffset(DiRo.Vector().ForwardVector * -60, true);

	DiRo = (GetActorLocation() - _ACKActor->GetActorLocation()).Rotation();
	DiRo.Pitch = 0;

	Health -= _Damage;

	if (Health <= 0)
	{
		Destroy();
		Cast<AInGame>(GetWorld()->GetLevelScriptActor())->GetMapgen()->DestroyMonster();
	}
}

