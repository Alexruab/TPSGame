// My game copyright

#if WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS

#include "TPS_5_7_3/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS_5_7_3//Components/TPSInventoryComponent.h"
#include "TPS_5_7_3//TPSTypes.h"
#include "TPS_5_7_3/Tests/TestUtils.h"

using namespace TPS::Test;

namespace
{
	class UTPSInventoryComponentTestable : public UTPSInventoryComponent
	{
		public:
		void SetLimits(const TMap<EInventoryItemType, int32>& Limits)
		{
			InventoryLimits = Limits;
		}
	};
	
	TMap<EInventoryItemType, int32> InitLimits(UTPSInventoryComponentTestable* InvComp, int32 ScoreLimit)
	{
		TMap<EInventoryItemType, int32> InvLimits;
		ENUM_LOOP_START(EInventoryItemType, EnumElem)
		InvLimits.Add(EnumElem, ScoreLimit);
		ENUM_LOOP_END
		
		InvComp->SetLimits(InvLimits);
		return InvLimits;
	}
	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "TPSGame.Components.Inventory.ComponentCouldBeCreated",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault, "TPSGame.Components.Inventory.ItemScoresShouldBeZerosByDefault",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoresShouldntBeAdded, "TPSGame.Components.Inventory.NegativeScoresShouldntBeAdded",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoresShouldBeAdded, "TPSGame.Components.Inventory.PositiveScoresShouldBeAdded",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
	
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoreThenScoresLimitShouldntBeAdded, "TPSGame.Components.Inventory.MoreThenScoresLimitShouldntBeAdded",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
	const UTPSInventoryComponent* InventoryComponent = NewObject<UTPSInventoryComponent>();
	TestNotNull("Inventory component exists", InventoryComponent);
	
	return true;
}

bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
	const UTPSInventoryComponent* InventoryComponent = NewObject<UTPSInventoryComponent>();
	TestNotNull("Inventory component exists", InventoryComponent);
	
	ENUM_LOOP_START(EInventoryItemType, EnumElem)
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 0);
	ENUM_LOOP_END
	
	return true;
}

bool FNegativeScoresShouldntBeAdded::RunTest(const FString& Parameters)
{
	UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;
	
	InitLimits(InventoryComponent, 100);
	
	const int32 NegativeScoreAmount = -10;
	const int32 InitialScoreAmount = 10;
	ENUM_LOOP_START(EInventoryItemType, EnumElem)
	TestTrueExpr(InventoryComponent->TryToAddItem({EnumElem, InitialScoreAmount}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialScoreAmount);
	TestFalseExpr(InventoryComponent->TryToAddItem({EnumElem, NegativeScoreAmount}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialScoreAmount);
	ENUM_LOOP_END
	
	return true;
}

bool FPositiveScoresShouldBeAdded::RunTest(const FString& Parameters)
{
	UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;
	
	InitLimits(InventoryComponent, 300);
	
	TestTrueExpr(InventoryComponent->TryToAddItem({EInventoryItemType::SPHERE, 10}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);
	
	return true;
}

bool FMoreThenScoresLimitShouldntBeAdded::RunTest(const FString& Parameters)
{
	UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
	if (!TestNotNull("Inventory component exists", InventoryComponent)) return false;
	
	const int32 ScoreLimit = 100;
	InitLimits(InventoryComponent, ScoreLimit);
	
	TestTrueExpr(InventoryComponent->TryToAddItem({EInventoryItemType::SPHERE, 10}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);
	
	TestFalseExpr(InventoryComponent->TryToAddItem({EInventoryItemType::SPHERE, ScoreLimit + 1}));
	TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EInventoryItemType::SPHERE) == 10);
	
	return true;
}

#endif
