// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem.h"
#include "Level/InGame.h"
#include "HUD/MyHUD.h"

ACardSystem::ACardSystem()
{
}

ACardSystem::~ACardSystem()
{
}

void ACardSystem::RotBAll()
{
	if (RotCounter < 3)
	{
		CardActorArray[RotCounter]->ShowB();

		RotCounter++;
	}
	else
	{
		GetWorldTimerManager().SetTimer(MoveTimer, this, &ACardSystem::MoveAll, 0.2, false, 0.2);

		RotCounter = 0;
	}
}

void ACardSystem::MoveAll()
{
	for (int i = 0; i < 3; i++)
	{
		CardActorArray[i]->MoveToSuffle();
	}
}

void ACardSystem::RotFAll()
{
	for (int i = 0; i < 3; i++)
	{
		CardActorArray[i]->ShowF();
	}
}

void ACardSystem::MoveToField()
{
	RotCounter++;

	if (RotCounter >= 3)
	{
		TArray<int> CardNum;

		for (int i = 0; i < 3; i++)
		{
			int R = 0;
			for (;;)
			{
				bool Check = true;
				R = rand() % 3;

				for (int i = 0; i < CardNum.Num(); i++)
				{
					if (CardNum[i] == R)
						Check = false;
				}

				if (Check)
				{
					CardNum.Add(R);
					break;
				}
			}

			CardActorArray[i]->MoveToNum(R);

			UE_LOG(LogTemp, Log, TEXT("Move to Number %d %d"), i, R);
		}
	}
}

void ACardSystem::SpawnCard()
{
	LevelScript = Cast<AInGame>(GetWorld()->GetLevelScriptActor());

	for (int i = 0; i < 3; i++)
	{
		CardActorArray.Add(
			LevelScript->GetWorld()->SpawnActor<ACardActor>(
				ACardActor::StaticClass(),
				FTransform(
					FRotator(-90, -90, 0),
					FVector(-600 + (600 * i), 0, -1400),
					FVector(1, 1, 1))));

		int BuffNum = rand() % LevelScript->GetMyGameInstance()->BuffData.Num();
		int DeBuffNum = rand() % LevelScript->GetMyGameInstance()->DeBuffData.Num();

		CardActorArray[i]->SetCard(
			LevelScript->GetMyGameInstance()->BuffData[BuffNum],
			LevelScript->GetMyGameInstance()->DeBuffData[DeBuffNum]);

		CardActorArray[i]->SetCardSystem(this);
	}

	Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->CardSelectWidgetChangeButton(0);

	RotCounter = 0;
}

void ACardSystem::DestroyCard()
{
	for (int i = 0; i < CardActorArray.Num(); i++)
	{
		CardActorArray[i]->Destroy();
	}

	CardActorArray.Empty();
}

void ACardSystem::InitCard()
{
	for (int i = 0; i < TimeHandlerArray.Num(); i++)
	{
		GetWorldTimerManager().ClearTimer(TimeHandlerArray[i]);
	}

	TimeHandlerArray.Empty();

	RotCounter = 0;
}

void ACardSystem::AddCard(FCardDataTableRow _Buff, FCardDataTableRow _DeBuff)
{
	for (int i = 0; i < 3; i++)
	{
		CardActorArray[i]->SetCanClick(false);
	}

	CardArray.Add(_Buff);
	CardArray.Add(_DeBuff);

	for (int i = 0; i < CardArray.Num(); i++)
	{
		switch (CardArray[i].CardType)
		{
		case E_CardType::C_SpawnType:
			SpawnType(CardArray[i]);
			break;

		case E_CardType::C_TickType:
			TickType(CardArray[i]);
			break;

		case E_CardType::C_StatType:
			StatType(CardArray[i]);
			break;

		case E_CardType::C_AddDamageType:
			AddDamageType(CardArray[i]);
			break;

		case E_CardType::C_AvoidType:
			AvoidType(CardArray[i]);
			break;
		}
	}
}

void ACardSystem::SpawnType(FCardDataTableRow _Data)
{
	switch (_Data.SpawnType)
	{
	case E_C_SpawnType::C_S_GhostType:
		break;

	case E_C_SpawnType::C_S_ObjectType:
		DelegateArray.Add(FTimerDelegate::CreateUObject(this, &ACardSystem::SpawnObject, _Data.Mesh));
		TimeHandlerArray.Add(FTimerHandle());

		int Num = TimeHandlerArray.Num() - 1;
		GetWorldTimerManager().SetTimer(TimeHandlerArray[Num], DelegateArray[Num], _Data.LoopTime, _Data.Loop, _Data.LoopTime);
		break;
	}
}

void ACardSystem::TickType(FCardDataTableRow _Data)
{
	switch (_Data.EffectType)
	{
	case E_C_EffectType::C_E_DamageType:
		break;

	case E_C_EffectType::C_E_MoneyType:
		break;
	}
}

void ACardSystem::AddDamageType(FCardDataTableRow _Data)
{
}

void ACardSystem::StatType(FCardDataTableRow _Data)
{
}

void ACardSystem::AvoidType(FCardDataTableRow _Data)
{
}

void ACardSystem::SpawnObject(UStaticMesh* _Mesh)
{

}
