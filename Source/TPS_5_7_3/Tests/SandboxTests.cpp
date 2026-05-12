// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FMathMaxInt, "TPSGame.FMath.MaxInt",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FMathSqrt, "TPSGame.FMath.Sqrt",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);


bool FMathMaxInt::RunTest(const FString& Parameters)
{
	TestTrue("2 different positive numbers", FMath::Max(13, 26) == 26);
	
	TestEqual("2 equal positive numbers", FMath::Max(5, 5), 5);
	
	TestEqual("positive and negative numbers", FMath::Max(456, -43), 456);
	
	TestTrueExpr(FMath::Max(4, 0) == 4);
	
	TestEqual("2 different negative numbers", FMath::Max(-47, -50), -47);
	
	TestEqual("2 equal negative numbers", FMath::Max(-74, -74), -74);
	
	TestEqual("2 zero numbers", FMath::Max(0, 0), 0);
	
	TestEqual("negative and zero numbers", FMath::Max(0, -8), 0);
	
	return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt function testing");
	
	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
	
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f, 0.1f);
	
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.73f, 0.01f);
	
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);
	
	return true;
}

#endif


