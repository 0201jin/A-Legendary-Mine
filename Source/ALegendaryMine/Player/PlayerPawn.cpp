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

void APlayerPawn::FB_Move(float _value)
{
	FVector Direction = GetActorLocation().ForwardVector;//FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, _value);
}

void APlayerPawn::LR_Move(float _value)
{
	FVector Direction = GetActorLocation().RightVector;//FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, _value);
}
