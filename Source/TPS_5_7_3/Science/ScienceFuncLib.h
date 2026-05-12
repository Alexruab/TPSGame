// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"


UCLASS()
class TPS_5_7_3_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	public:
	UFUNCTION(BlueprintPure, Category = "Science")
	static int32 Fibonacci(int32 Value);
	
	UFUNCTION(BlueprintPure, Category = "Science")
	static int32 FibonacciWithoutRecursion(int32 Value);
};
