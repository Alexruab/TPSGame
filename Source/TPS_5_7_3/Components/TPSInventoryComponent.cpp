// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TPSInventoryComponent.h"

UTPSInventoryComponent::UTPSInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}

bool UTPSInventoryComponent::TryToAddItem(const FInventoryData& Data)
{
	if (Data.Score < 0) return false;
	
	if (!Inventory.Contains(Data.Type))
	{
		Inventory.Add(Data.Type, 0);
	}
	
	const auto NextScore = Inventory[Data.Type] + Data.Score; 
	
	if (NextScore > InventoryLimits[Data.Type]) return false;
	
	Inventory[Data.Type] += Data.Score;
	
	return true;

}

int32 UTPSInventoryComponent::GetInventoryAmountByType(EInventoryItemType Type) const
{
	return Inventory.Contains(Type) ? Inventory[Type] : 0;
}

void UTPSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
	ensure(InvEnum);
	for (int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)
	{
		const EInventoryItemType EnumElem = static_cast<EInventoryItemType>(i);
		const FString EnumElemName = UEnum::GetValueAsString(EnumElem);
		const bool LimitCheckCond = InventoryLimits.Contains(EnumElem) && InventoryLimits[EnumElem] >= 0;
		ensureMsgf(LimitCheckCond, TEXT("Limit for %s does not exist."), *EnumElemName);
	}
	
}


