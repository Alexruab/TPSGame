// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS_5_7_3/Tests/TPSInventoryItemTests.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>
//#include <ThirdParty/WebRTC/5414/Include/rtc_base/logging.h>

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS_5_7_3/Items/TPSInventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DSP/BufferDiagnostics.h"
#include "TPS_5_7_3/Tests/TestUtils.h"
#include "Misc/OutputDevice.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_5_7_3/TPS_5_7_3Character.h"
#include "TPS_5_7_3/Components/TPSInventoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);
	
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryInventoryItemMeshExist, "TPSGame.Items.Inventory.EveryInventoryItemMeshExist",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
	constexpr const char* InventoryItemBPName = "Blueprint'/Game/Items/BP_TPSInventoryItem.BP_TPSInventoryItem'";
	constexpr const char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSInventoryItem_Child.BP_Test_TPSInventoryItem_Child'";
	
}

using namespace TPS::Test;

///Script/Engine.World'/Game/Tests/EmptyTestLevel.EmptyTestLevel'
// World /Script/Engine.World'/Game/ThirdPerson/Lvl_ThirdPerson.Lvl_ThirdPerson'

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
	const FString ExpectedWarningMsg =
		FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
	AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);
	
	LevelScope("/Game/Tests/EmptyTestLevel");
	//AutomationOpenMap("/Game/Tests/EmptyTestLevel");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	
	const FTransform InitialTransform{FVector{1000.0f}};
	const ATPSInventoryItem* InvItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitialTransform);
	if (!TestNull("Inventory component exist",InvItem)) return false;
	
	return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	LevelScope("/Game/Tests/EmptyTestLevel");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	const FTransform InitialTransform{FVector{1000.0f}};
	const auto InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPName, InitialTransform);
	
	if (!TestNotNull("Inventory component exist",InvItem)) return false;
	
	const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Collision exist", CollisionComp)) return false;
	
	TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);
	
	ENUM_LOOP_START(ECollisionChannel, EElem)
	if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
	{
		TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) ==  ECR_Overlap);
	}
	ENUM_LOOP_END
	
	const auto TextRendComp = InvItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("TextRenderComponent exist", TextRendComp)) return false;
	
	const auto StaticMechComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("StaticMeshComponent exist", StaticMechComp)) return false;
	
	TestTrueExpr(StaticMechComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);
	
	return true;
}

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	AutomationOpenMap("/Game/Tests/EmptyTestLevel");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	const FTransform InitialTransform{FVector{1000.0f}};
	const auto InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
	
	if (!TestNotNull("Inventory component exist",InvItem)) return false;
	
	const int32 Score = 13;
	const FInventoryData InvData{EInventoryItemType::CYLINDER, Score};
	const FLinearColor Color = FLinearColor::Yellow;
	CallFuncByNameWithParams(InvItem, "SetInventoryData",{InvData.ToString(), Color.ToString()});
	
	const auto TextRendComp = InvItem->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("TextRenderComponent exist", TextRendComp)) return false;
	
	TestTrueExpr(TextRendComp->Text.ToString().Equals(FString::FromInt(Score)));
	UE_LOG(LogTemp, Warning, TEXT("%s == %s"), *TextRendComp->Text.ToString(), *FString::FromInt(Score));
	
	TestTrueExpr(TextRendComp->TextRenderColor == Color.ToFColor(true));
	UE_LOG(LogTemp, Warning, TEXT("%s == %s"), *TextRendComp->TextRenderColor.ToString(), *Color.ToFColor(true).ToString());
	
	const auto StaticMechComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("StaticMeshComponent exist", StaticMechComp)) return false;
	
	const auto Material = StaticMechComp->GetMaterial(0);
	if (!TestNotNull("Material exist", Material)) return false;
	
	FLinearColor MaterialColor;
	Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialColor);
	TestTrueExpr(MaterialColor ==  Color);
	UE_LOG(LogTemp, Warning, TEXT("%s == %s"), *MaterialColor.ToString(), *Color.ToString());
	
	return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
	AutomationOpenMap("/Game/Tests/EmptyTestLevel");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	const FTransform InitialTransform{FVector{1000.0f}};
	const auto InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
	
	if (!TestNotNull("Inventory component exist",InvItem)) return false;
	
	const int32 Score = 13;
	const EInventoryItemType Type = EInventoryItemType::CYLINDER;
	const FInventoryData InvData{Type, Score};
	const FLinearColor Color = FLinearColor::Yellow;
	CallFuncByNameWithParams(InvItem, "SetInventoryData",{InvData.ToString(), Color.ToString()});
	
	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, ATPS_5_7_3Character::StaticClass(), Pawns);
	if (!TestTrueExpr(Pawns.Num() == 1)) return false;
	
	const auto Character = Cast<ATPS_5_7_3Character>(Pawns[0]);
	if (!TestNotNull("Character exist", Character)) return false;
	
	const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
	if (!TestNotNull("Inventory component exist", InvComp)) return false;
	
	TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == 0);
	
	// Character takes inventory item
	
	Character->SetActorLocation(InitialTransform.GetLocation());
	
	TestTrueExpr(InvComp->GetInventoryAmountByType(Type) == Score);
	TestTrueExpr(!IsValid(InvItem));
	
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), Items);
	TestTrueExpr(Items.Num() == 0);
	
	return true;
}

bool FEveryInventoryItemMeshExist::RunTest(const FString& Parameters)
{
	AutomationOpenMap("/Game/Tests/EmptyTestLevel");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	
	ENUM_LOOP_START(EInventoryItemType, EElem);
	const FTransform InitialTransform{FVector{100.0f * (index + 1)}};
	auto InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitialTransform);
	if (!TestNotNull("Inventory component exist",InvItem)) return false;
	
	const int32 Score = 13;
	const FInventoryData InvData{EElem, Score};
	const FLinearColor Color = FLinearColor::Red;
	CallFuncByNameWithParams(InvItem, "SetInventoryData",{InvData.ToString(), Color.ToString()});
	
	const auto StaticMechComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("StaticMeshComponent exist", StaticMechComp)) return false;
	
	const FString MeshMsg = FString::Printf(TEXT("Static Mesh for %s exist"), *UEnum::GetValueAsString(EElem));
	
	TestNotNull(*MeshMsg, StaticMechComp->GetStaticMesh().Get());
	
	ENUM_LOOP_END;
	
	
	return true;
}

#endif
