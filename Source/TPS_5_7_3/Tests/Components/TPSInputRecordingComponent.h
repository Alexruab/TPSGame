// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS_5_7_3/Tests/Utils/InputRecordingsUtils.h"
#include "TPSInputRecordingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_5_7_3_API UTPSInputRecordingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSInputRecordingComponent();

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	FInputData InputData;
	
	FBindingsData MakeBindingsData() const;
};
