// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GridPlacementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(GridPlacementComponent, All, All);

// Sets default values for this component's properties
UGridPlacementComponent::UGridPlacementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGridPlacementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(GetWorld(), TEXT("world pointer not exist"));
	// ...
	
}


bool UGridPlacementComponent::GetGridPositionFromCamera(FVector& OutGridLocation, FRotator& OutGridRotation)
{
	auto PlayerCameraManager = Cast<APlayerCameraManager>(GetOwner());
	
	if (!PlayerCameraManager && Cast<APawn>(GetOwner()))
	{
		PlayerCameraManager = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController())->PlayerCameraManager;
	}
	
	if (!PlayerCameraManager) return false;
	
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = TraceDistance * ForwardVector + CameraLocation;
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel( HitResult, CameraLocation, End, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		
		/*
		if (HitResult.GetActor()->IsA<ATPSBaseBuild>())
		{
			
			UE_LOG(GridPlacementComponent, Log, TEXT("%s"), *GridCenterPosition(HitResult.Location).ToString());
		}
		*/
	}
	
	return true;
	
}

FVector UGridPlacementComponent::GridCenterPosition(const FVector& HitLocation) const
{
	float NewLocationX = FMath::FloorToInt(HitLocation.X / GridSize) * GridSize + (GridSize / 2.0f);
	float NewLocationY = FMath::FloorToInt(HitLocation.Y / GridSize) * GridSize + (GridSize / 2.0f);
	
	return FVector(NewLocationX, NewLocationY, HitLocation.Z);
}

// Called every frame
void UGridPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector OutGridLocation;
	FRotator OutGridRotation;
	GetGridPositionFromCamera(OutGridLocation, OutGridRotation);
}

