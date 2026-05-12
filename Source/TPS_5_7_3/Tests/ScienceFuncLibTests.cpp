// Fill out your copyright notice in the Description page of Project Settings.
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/ScienceFuncLibTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS_5_7_3/Science/ScienceFuncLib.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacciSimple, "TPSGame.Science.Fibonacci.Simple",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
	
IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacciStress, "TPSGame.Science.Fibonacci.Stress",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacciWithoutRecursionStress, "TPSGame.Science.FibonacciWithoutRecursion.Stress",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacciLogHasErrors, "TPSGame.Science.Fibonacci.LogHasErrors",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci simple testing");
	TestEqual("positive number", UScienceFuncLib::Fibonacci(2), 1);
	TestEqual("first number", UScienceFuncLib::Fibonacci(0), 0);
	TestEqual("second number", UScienceFuncLib::Fibonacci(1), 1);
	TestEqual("positive number", UScienceFuncLib::Fibonacci(6), 8);
	
	struct TestPayload
	{
		int32 TestValue;
		int32 ExpectedValue;
	};
	
	const TArray<TestPayload> TestData ={{0,0}, //
		{1, 1}, //
		{2, 1}, //
		{3, 2}, //
		{4, 3},
		{5, 5}};
	
	for (const auto Data : TestData)
	{
		const FString InfoString = FString::Printf(TEXT("Test value: %i, expect value %i"), Data.TestValue, Data.ExpectedValue);
		TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
	}
	return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci stress testing");
	int32 PrevPrevValue = 0;
	int32 PrevValue = 1;
	
	for (int32 i = 2; i < 40; ++i)
	{
		const int32 NewValue = UScienceFuncLib::Fibonacci(i);
		TestTrueExpr(NewValue == PrevPrevValue + PrevValue);
		
		PrevPrevValue = PrevValue;
		PrevValue = NewValue;
	}
	
	return true;
}

bool FFibonacciWithoutRecursionStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci without recursion testing");
	int32 PrevPrevValue = 0;
	int32 PrevValue = 1;
	
	for (int32 i = 2; i < 40; ++i)
	{
		const int32 NewValue = UScienceFuncLib::FibonacciWithoutRecursion(i);
		TestTrueExpr(NewValue == PrevPrevValue + PrevValue);
		
		PrevPrevValue = PrevValue;
		PrevValue = NewValue;
	}
	
	return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci negative on input produce error");
	
	AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
	UScienceFuncLib::Fibonacci(-10);
	
	return true;
}

#endif
