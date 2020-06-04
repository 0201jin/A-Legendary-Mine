// Fill out your copyright notice in the Description page of Project Settings.


#include "CardActor.h"
#include "Engine.h"
#include "PaperSprite.h"
#include "CardSystem.h"
#include "HUD/MyHUD.h"

// Sets default values
ACardActor::ACardActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UPaperSprite* B_PaperSpriteAsset = LoadObject<UPaperSprite>(NULL, TEXT("PaperSprite'/Game/Card/Texture/Card_Back_Sprite.Card_Back_Sprite'"), NULL, LOAD_None, NULL);
	UPaperSprite* F_PaperSpriteAsset = LoadObject<UPaperSprite>(NULL, TEXT("PaperSprite'/Game/Card/Texture/Card_Front_Sprite.Card_Front_Sprite'"), NULL, LOAD_None, NULL);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	FrontSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("F_Sprite"));
	FrontSpriteComponent->SetupAttachment(RootComponent);
	FrontSpriteComponent->SetSprite(F_PaperSpriteAsset);
	FrontSpriteComponent->SetRelativeRotation(FRotator(0, 90, 0));

	BackSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("B_Sprite"));
	BackSpriteComponent->SetupAttachment(RootComponent);
	BackSpriteComponent->SetSprite(B_PaperSpriteAsset);
	BackSpriteComponent->SetRelativeLocation(FVector(0.1, 0, 0));
	BackSpriteComponent->SetRelativeRotation(FRotator(0, 90, 0));

	Buff = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Buff"));
	Buff->SetupAttachment(FrontSpriteComponent);
	Buff->SetRelativeRotation(FRotator(0, 90, 0));
	Buff->SetRelativeLocation(FVector(-220, 0.1, 320));
	Buff->SetText("BUFF");
	Buff->SetTextRenderColor(FColor::Black);

	Buff_Explain = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Buff_Explain"));
	Buff_Explain->SetupAttachment(FrontSpriteComponent);
	Buff_Explain->SetRelativeRotation(FRotator(0, 90, 0));
	Buff_Explain->SetRelativeLocation(FVector(-220, 0.1, 250));
	Buff_Explain->SetText("");
	Buff_Explain->SetTextRenderColor(FColor::Black);

	DeBuff = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DeBuff"));
	DeBuff->SetupAttachment(FrontSpriteComponent);
	DeBuff->SetRelativeRotation(FRotator(0, 90, 0));
	DeBuff->SetRelativeLocation(FVector(-220, 0.1, 0));
	DeBuff->SetText("DEBUFF");
	DeBuff->SetTextRenderColor(FColor::Black);

	DeBuff_Explain = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DeBuff_Explain"));
	DeBuff_Explain->SetupAttachment(FrontSpriteComponent);
	DeBuff_Explain->SetRelativeRotation(FRotator(0, 90, 0));
	DeBuff_Explain->SetRelativeLocation(FVector(-220, 0.1, -60));
	DeBuff_Explain->SetText("");
	DeBuff_Explain->SetTextRenderColor(FColor::Black);
}

void ACardActor::SetCardSystem(ACardSystem* _CardSystem)
{
	CardSystem = _CardSystem;
}

void ACardActor::MoveToNum(int _Num)
{
	if (!GetWorldTimerManager().IsTimerActive(MoveTimer))
	{
		FTimerDelegate Dele = FTimerDelegate::CreateUObject(this, &ACardActor::MoveToNum, _Num);

		GetWorldTimerManager().SetTimer(MoveTimer, Dele, 0.002, true, 0);
	}

	AddActorWorldOffset(FVector(0, 1, 0));

	switch (_Num)
	{
	case 0:
		if (GetActorLocation().X >= -600)
			AddActorWorldOffset(FVector(-1, 0, 0));
		break;

	case 1:
		break;

	case 2:
		if (GetActorLocation().X <= 600)
			AddActorWorldOffset(FVector(1, 0, 0));
		break;
	}

	if (GetActorLocation().Y >= 0)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		CanClick = true;
	}
}

// Called when the game starts or when spawned
void ACardActor::BeginPlay()
{
	Super::BeginPlay();

	Location = GetActorLocation();
}

void ACardActor::Rot(float _Ro)
{
	AddActorLocalRotation(FRotator(0, 1, 0));

	int Ro = GetActorRotation().Pitch;

	if (Ro == _Ro)
	{
		GetWorldTimerManager().ClearTimer(RotTimer);

		GetWorldTimerManager().SetTimer(MoveTimer, this, &ACardActor::MoveToUp, 0.002, true, 0);
	}
}

void ACardActor::RotAndSuffle(float _Ro)
{
	AddActorLocalRotation(FRotator(0, 1, 0));

	int Ro = GetActorRotation().Pitch;

	if (Ro == _Ro)
	{
		GetWorldTimerManager().ClearTimer(RotTimer);

		CardSystem->RotBAll();
	}
}

void ACardActor::MoveSuffle()
{
	AddActorWorldOffset(FVector(0, -1, 0));

	if (GetActorLocation().X > 0)
	{
		AddActorWorldOffset(FVector(-1, 0, 0));
	}
	else if (GetActorLocation().X < 0)
	{
		AddActorWorldOffset(FVector(1, 0, 0));
	}

	if (GetActorLocation().Y < -750)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);

		CardSystem->MoveToField();
	}
}

void ACardActor::MoveToUp()
{
	AddActorWorldOffset(FVector(0, 0, 0.5));

	if (GetActorLocation().X > 0)
	{
		AddActorWorldOffset(FVector(-1, 0, 0));
	}
	else if (GetActorLocation().X < 0)
	{
		AddActorWorldOffset(FVector(1, 0, 0));
	}

	if (GetActorLocation().Z >= -1200)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
	}
}

void ACardActor::NotifyActorOnClicked(FKey PressedButton)
{
	static bool ShowB = false;

	if (CanClick)
	{
		CanClick = false;

		CardSystem->AddCard(CardArray[0], CardArray[1]);

		ShowF();

		//위젯에서 버튼 바꾸는 코드
		Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->CardSelectWidgetChangeButton(1);
	}
	else if (ShowB && ShowBig)
	{
		SetActorLocation(FVector(Location.X, Location.Y, -1200));
		ShowB = false;
	}
	else if (!ShowB && ShowBig)
	{
		SetActorLocation(Location);
		ShowB = true;
	}
}

// Called every frame
void ACardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACardActor::SetCard(FCardDataTableRow _Buff, FCardDataTableRow _DeBuff)
{
	CardArray.Add(_Buff);
	CardArray.Add(_DeBuff);

	Buff_Explain->SetText(_Buff.Explain);
	DeBuff_Explain->SetText(_DeBuff.Explain);
}

TArray<FCardDataTableRow> ACardActor::GetCard()
{
	return CardArray;
}

void ACardActor::ShowF()
{
	FTimerDelegate Dele = FTimerDelegate::CreateUObject(this, &ACardActor::Rot, -90.0f);

	GetWorldTimerManager().SetTimer(RotTimer, Dele, 0.002, true, 0);
}

void ACardActor::ShowB() //ShowB이지만 셔플하는 함수
{
	FTimerDelegate Dele = FTimerDelegate::CreateUObject(this, &ACardActor::RotAndSuffle, 90.0f);

	GetWorldTimerManager().SetTimer(RotTimer, Dele, 0.002, true, 0);

	Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->CardSelectWidgetChangeButton(2);

	SetActorLocation(Location);

	ShowBig = false;
}

void ACardActor::MoveToSuffle()
{
	GetWorldTimerManager().SetTimer(MoveTimer, this, &ACardActor::MoveSuffle, 0.002, true, 0);
}

