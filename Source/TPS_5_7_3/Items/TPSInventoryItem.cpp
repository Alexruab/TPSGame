// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TPSInventoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "TPS_5_7_3/Components/TPSInventoryComponent.h"

// Sets default values
ATPSInventoryItem::ATPSInventoryItem()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	check(CollisionComponent);
	
	CollisionComponent->InitSphereRadius(30.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionComponent);
}

void ATPSInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (const auto Pawn = Cast<APawn>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is already overlap!"));
		if (const auto InvComp = Pawn->FindComponentByClass<UTPSInventoryComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("FindComponentByClass<UTPSInventoryComponent"));
			if (InvComp->TryToAddItem(InventoryData))
			{
				UE_LOG(LogTemp, Warning, TEXT("TryToAddItem true"));
				Destroy();
			}
		}
		
	}
}



