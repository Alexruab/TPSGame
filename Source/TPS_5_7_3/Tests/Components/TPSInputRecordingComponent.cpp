// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/Components/TPSInputRecordingComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UTPSInputRecordingComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner())
	check(GetWorld())
	check(GetOwner()->InputComponent)
	InputData.InitialTransform = GetOwner()->GetActorTransform();
	InputData.Bindings.Add(MakeBindingsData());
}

void UTPSInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}



void UTPSInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

FBindingsData UTPSInputRecordingComponent::MakeBindingsData() const
{
	FBindingsData BindingsData;
	
	BindingsData.WorldTime = GetWorld()->TimeSeconds;
	for (const auto AxisBindings :)
}

