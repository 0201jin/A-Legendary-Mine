// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem.h"
#include "Level/InGame.h"

ACardSystem::ACardSystem()
{
}

ACardSystem::~ACardSystem()
{
}

void ACardSystem::InitCard()
{
	for (int i = 0; i < TimeHandlerArray.Num(); i++)
	{
		GetWorldTimerManager().ClearTimer(TimeHandlerArray[i]);
	}

	TimeHandlerArray.Empty();
}

void ACardSystem::AddCard(FCardDataTableRow _Buff, FCardDataTableRow _DeBuff)
{
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
