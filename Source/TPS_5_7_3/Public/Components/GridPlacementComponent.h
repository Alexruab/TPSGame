// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/TPSBaseBuild.h"
#include "GridPlacementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_5_7_3_API UGridPlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridPlacementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid Setting", meta=( ClampMin = "0.0", ClampMax = "1000.0", Multiple = "100.0", Delta = "100.0" ))
	float GridSize = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid Setting")
	float TraceDistance = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Grid Setting")
	TSubclassOf<AActor> PreviewActorClass;
	
	UFUNCTION(BlueprintCallable, Category= "Grid Setting")
	bool GetGridPositionFromCamera(FVector& OutGridLocation, FRotator& OutGridRotation);

private:
	FVector GridCenterPosition(const FVector& HitLocation) const;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
