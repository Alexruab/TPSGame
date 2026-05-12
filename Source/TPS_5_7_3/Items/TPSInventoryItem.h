// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_5_7_3/TPSTypes.h"
#include "TPSInventoryItem.generated.h"

class USphereComponent;

UCLASS()
class TPS_5_7_3_API ATPSInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ATPSInventoryItem();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent * CollisionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryData InventoryData;
};
