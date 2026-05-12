// Fill out your copyright notice in the Description page of Project Settings.


#include "Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScienceFuncLib, All, All);

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
	if (Value < 0)
	{
		UE_LOG(LogScienceFuncLib, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
	}
	return Value <= 1 ? Value : Fibonacci(Value - 2) + Fibonacci(Value - 1);
}

int32 UScienceFuncLib::FibonacciWithoutRecursion(int32 Value)
{
	if (Value < 0)
	{
		UE_LOG(LogScienceFuncLib, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
	}
	
	int32 FirstValue = 0;
	int32 SecondValue = 1;
	if (Value <= 1)
	{
		return Value;
	}
	
	for (int32 i = 2; i <= Value; ++i)
	{
		const int32 NextValue = FirstValue + SecondValue;
		FirstValue = SecondValue;
		SecondValue = NextValue;
	}
	return SecondValue;
}
