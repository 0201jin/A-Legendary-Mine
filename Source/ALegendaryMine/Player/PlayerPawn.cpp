// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Monster/MonsterActor.h"
#include "HitBox/HitBox.h"

#include "Boss/CircleSkillRange.h"

void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bJumping && bGodMode)
	{
		if (Cast<AMonsterActor>(OtherActor))
			Cast<AMonsterActor>(OtherActor)->JumpDamage(5, this);
	}
}

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("Pawn");
	Cast<UCapsuleComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); //값을 올리면 옆으로 흐르듯 이동
	GetCharacterMovement()->JumpZVelocity = 0.0f;
	GetCharacterMovement()->MaxStepHeight = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->AirControl = 0.0f;
	GetCharacterMovement()->GroundFriction = 100.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->TargetOffset = FVector(0, 0, 700);
	CameraBoom->ProbeSize = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetRelativeLocation(FVector(0, 0, 0));
	FollowCamera->SetRelativeRotation(FRotator(290, 0, 0));

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bone"));
	Weapon->SetRelativeRotation(FRotator(0, 0, -90));
	Weapon->SetRelativeScale3D(FVector(0.02, 0.02, 0.02));
	Weapon->CastShadow = true;
	Weapon->SetCollisionProfileName("OverlapAll");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAttack)
	{
		FVector Loca = GetActorLocation() + (GetActorForwardVector() * HitBoxY);
		HitBox->SetActorLocation(Loca);

		FRotator DiRo = ((Weapon->GetComponentToWorld().GetLocation()) - GetActorLocation()).Rotation();
		DiRo.Pitch = 90;
		DiRo.Roll = 0;

		HitBox->SetActorRotation(DiRo);

		HitBox->SetCollisionEnable(ECollisionEnabled::QueryAndPhysics);
	}
}

void APlayerPawn::SetRot(FRotator _Ro)
{
	if (CheckAction())
		SetActorRotation(_Ro);
}

void APlayerPawn::FB_Move(float _value)
{
	if (CheckAction())
	{
		fFBValue = _value;
		FVector Direction = GetActorLocation().ForwardVector;
		AddMovementInput(Direction, _value);
	}
}

void APlayerPawn::LR_Move(float _value)
{
	if (CheckAction())
	{
		fLRValue = _value;
		FVector Direction = GetActorLocation().RightVector;
		AddMovementInput(Direction, _value);
	}
}

void APlayerPawn::Jump()
{
	if (bJump && !bAttack && (fLRValue != 0 || fFBValue != 0))
	{
		Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("OverlapOnlyPawn");

		bJumping = true;
		bJump = false;

		FVector FB_Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		FVector LR_Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

		FB_Direction *= fFBValue;
		LR_Direction *= fLRValue;

		SetActorRotation((FB_Direction + LR_Direction).Rotation());

		GetMesh()->GetAnimInstance()->Montage_Play(JumpAnimation);

		GetWorldTimerManager().SetTimer(JumpTimer, this, &APlayerPawn::JumpTimerFunc, 0.01f, true, 0.0f);
		GetWorldTimerManager().SetTimer(JumpTimerEnd, this, &APlayerPawn::JumpTimerEndFunc, 0.417f, false, 0.417f);
	}
}

void APlayerPawn::JumpTimerFunc()
{
	AddActorLocalOffset(GetActorLocation().ForwardVector * 4, true);
	bGodMode = true;
}

void APlayerPawn::JumpTimerEndFunc()
{
	Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("Pawn");

	bGodMode = false;
	bJumping = false;
	bGodMode = false;

	GetWorldTimerManager().ClearTimer(JumpTimer);
	GetWorldTimerManager().SetTimer(JumpTimer, this, &APlayerPawn::JumpTimerCoolFunc, 0.3f, false, 0.3f);
}

void APlayerPawn::JumpTimerCoolFunc()
{
	bJump = true;
}

void APlayerPawn::Damage(int _Damage)
{
	UE_LOG(LogTemp, Log, TEXT("Player Damaged!"));
}
void APlayerPawn::Attack(FRotator _Ro)
{
	if (CheckAction())
	{
		SetActorRotation(_Ro);

		HitBox->ClearHitMonster();

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (!AnimInstance || !AttackAnimation) return;

		bAttack = true;

		const char* Combo[] = { "Attack1", "Attack2", "Attack3" };

		if (!AnimInstance->Montage_IsPlaying(AttackAnimation))
		{
			AnimInstance->Montage_Play(AttackAnimation);
		}
		else if (AnimInstance->Montage_IsPlaying(AttackAnimation))
		{
			AnimInstance->Montage_Play(AttackAnimation);
			AnimInstance->Montage_JumpToSection(FName(Combo[iComboCnt]), AttackAnimation);

			UE_LOG(LogTemp, Log, TEXT("Attack %d"), iComboCnt);
		}
	}
	else
	{
		bAttackWhenAttack = true;
	}
}

void APlayerPawn::CheckEndAttack()
{
	bAttack = false;

	if (!bAttackWhenAttack)
		iComboCnt = 0;

	HitBox->SetCollisionEnable(ECollisionEnabled::NoCollision);
	HitMonsterList.Empty();
}

void APlayerPawn::CheckInputAttack()
{
	if (iComboCnt >= 2)
	{
		iComboCnt = 0;
		bAttackWhenAttack = false;
	}
	else if (bAttackWhenAttack)
	{
		iComboCnt += 1;
		bAttackWhenAttack = false;
	}
}

void APlayerPawn::MoveAttack()
{
	GetWorldTimerManager().SetTimer(JumpTimer, this, &APlayerPawn::MoveTimer, 0.01f, true, 0.0f);
	GetWorldTimerManager().SetTimer(JumpTimerEnd, this, &APlayerPawn::JumpTimerEndFunc, 0.1f, false, 0.1f);
}

bool APlayerPawn::CheckAction()
{
	if (!bJumping && !bAttack)
		return true;

	return false;
}

void APlayerPawn::MoveTimer()
{
	AddActorLocalOffset(GetActorLocation().ForwardVector * 4, true);
}

void APlayerPawn::EndMoveTimer()
{
	GetWorldTimerManager().ClearTimer(JumpTimer);
}
