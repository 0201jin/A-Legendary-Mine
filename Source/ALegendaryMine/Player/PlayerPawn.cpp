// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Cast<UCapsuleComponent>(RootComponent)->SetCollisionProfileName("CharacterMesh");

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
	FollowCamera->RelativeLocation = FVector(0, 0, 0);
	FollowCamera->RelativeRotation = FRotator(290, 0, 0);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->AttachTo(GetMesh(), TEXT("Bone"));
	Weapon->RelativeRotation = FRotator(0, 0, -90);
	Weapon->RelativeScale3D = FVector(0.02, 0.02, 0.02);
	Weapon->CastShadow = true;
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

}

void APlayerPawn::SetRot(FRotator _Ro)
{
	if (!bJumping)
		SetActorRotation(_Ro);
}

void APlayerPawn::Attack()
{

}

void APlayerPawn::FB_Move(float _value)
{
	if (!bJumping)
	{
		fFBValue = _value;
		FVector Direction = GetActorLocation().ForwardVector;
		AddMovementInput(Direction, _value);
	}
}

void APlayerPawn::LR_Move(float _value)
{
	if (!bJumping)
	{
		fLRValue = _value;
		FVector Direction = GetActorLocation().RightVector;
		AddMovementInput(Direction, _value);
	}
}

void APlayerPawn::Jump()
{
	if (bJump && (fLRValue != 0 || fFBValue != 0))
	{
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
	bGodMode = false;
	bJumping = false;
	GetWorldTimerManager().ClearTimer(JumpTimer);
	GetWorldTimerManager().SetTimer(JumpTimer, this, &APlayerPawn::JumpTimerCoolFunc, 0.3f, false, 0.3f);
}

void APlayerPawn::JumpTimerCoolFunc()
{
	bJump = true;
}
