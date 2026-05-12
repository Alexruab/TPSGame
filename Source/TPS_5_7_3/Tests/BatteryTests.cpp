// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS_5_7_3/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS_5_7_3/Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "TPSGame.Items.Battery", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
	using namespace TPS;
	AddInfo("Battery with default constructor");
	const Battery BatteryDefault;
	
	TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
	TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
	TestTrueExpr(BatteryDefault.ToString().Equals("100%"));
	
	AddInfo("Battery with custom constructor");
	
	const auto BatteryTestFunc = [this](float Percent, const FColor Color, const FString& PercentString)
	{
		const Battery BatteryObject{Percent};
	
		TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
		TestTrueExpr(BatteryObject.GetColor() == Color);
		TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
	};
	
	
	BatteryTestFunc(1.0f, FColor::Green, "100%");
	BatteryTestFunc(0.45f, FColor::Yellow, "45%");
	BatteryTestFunc(0.1f, FColor::Red, "10%");
	BatteryTestFunc(100.0f, FColor::Green, "100%");
	BatteryTestFunc(-1000.0f, FColor::Red, "0%");
	
	AddInfo("Battery Charge / UnCharge");
	
	Battery BatteryObject{0.6f};
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
	
	BatteryObject.Charge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.7f, 0.0001f));
	BatteryObject.UnCharge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
	BatteryObject.UnCharge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
	
	BatteryObject.UnCharge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.4f));
	BatteryObject.Charge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
	BatteryObject.Charge();
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
	
	AddInfo("Battery Charge / UnCharge. Corner cases");
	
	for (int32 i = 0; i < 20; ++i)
	{
		BatteryObject.UnCharge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));
	
	for (int32 i = 0; i < 20; ++i)
	{
		BatteryObject.Charge();
	}
	TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));
	
	AddInfo("Battery comparison");
	
	Battery BatteryObject1{0.5f};
	Battery BatteryObject2{0.5f};
	TestTrueExpr(BatteryObject1.GetPercent() == BatteryObject2.GetPercent());
	BatteryObject2.UnCharge();
	TestTrueExpr(BatteryObject1.GetPercent() > BatteryObject2.GetPercent());
	TestFalseExpr(BatteryObject1.GetPercent() < BatteryObject2.GetPercent());
	
	AddInfo("Battery in memory comparison");
	
	TestNotSame("Not the same battery", BatteryObject1, BatteryObject2);
	
	Battery& BatteryObject1Ref = BatteryObject1;
	TestSame("the same battery", BatteryObject1, BatteryObject1Ref);
	
	return true;
}

#endif
