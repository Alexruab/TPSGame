// My game copyright

#if WITH_AUTOMATION_TESTS

#include "TPS_5_7_3/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS_5_7_3/Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_5_7_3/Items/TPSInventoryItem.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "TPSGame.Gameplay.InventoryItemCanBeTakenOnJump", 
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh", 
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement", 
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;


DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLetentCommand, ACharacter*, Character);

bool FJumpLetentCommand::Update()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
	if (!IsValid(EnhancedInputComponent)) return false;
	
	const int32 ActionIndex =  GetActionBindingIndexByName(EnhancedInputComponent, "IA_Jump", ETriggerEvent::Started);
	if (ActionIndex != INDEX_NONE)
	{
		const auto& Binding = EnhancedInputComponent->GetActionEventBindings()[ActionIndex];
		const FInputActionInstance ActionInstance(Binding->GetAction());
		Binding->Execute(ActionInstance);
	}
	return true;
}

bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/Tests/InventoryTestLevel1");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exist", Character)) return false;
	
	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
	if (!TestEqual("Only one actor exist",InventoryItems.Num(), 1)) return false;
	
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	//Jump
	
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLetentCommand(Character));
	
		
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([this, World]()
	{
		TArray<AActor*> InventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
		TestTrueExpr(InventoryItems.Num()== 0);
	}, 
	2.0f));
	
	return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/Tests/InventoryTestLevel2");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exist", Character)) return false;
	
	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
	if (!TestEqual("Only one actor exist",InventoryItems.Num(), 1)) return false;
	
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	//Jump
	
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLetentCommand(Character));
	
		
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([this, World]()
	{
		TArray<AActor*> InventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
		TestTrueExpr(InventoryItems.Num() == 1);
	}, 
	2.0f));
	
	return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
	const auto Level = LevelScope("/Game/Tests/InventoryTestLevel3");
	
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World)) return false;
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
	if (!TestNotNull("Character exist", Character)) return false;
	
	TArray<AActor*> InventoryItems;
	UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
	TestTrueExpr(InventoryItems.Num() == 8);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
	if (!IsValid(EnhancedInputComponent)) return false;
    			
	UEnhancedPlayerInput* EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(Character->GetController<APlayerController>()->PlayerInput);
	if (!IsValid(EnhancedPlayerInput)) return false;
	
	const FInputActionValue InputActionValueForward{FVector2D(0.0f, 1.0f)};
	
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));

	ADD_LATENT_AUTOMATION_COMMAND(FUntilCommand(
		[EnhancedInputComponent, EnhancedPlayerInput, InputActionValueForward]()
	{
		return CallInjectInputForAction(EnhancedInputComponent, EnhancedPlayerInput, "IA_Move", ETriggerEvent::Triggered, InputActionValueForward);
	}, 
	[](){return true;}, 
	3.0f));
	
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	ADD_LATENT_AUTOMATION_COMMAND(FJumpLetentCommand(Character));
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
	
	const FInputActionValue InputActionValueRight{FVector2D(1.0f, 0.0f)};
	
	ADD_LATENT_AUTOMATION_COMMAND(FUntilCommand(
		[EnhancedInputComponent, EnhancedPlayerInput, InputActionValueRight]()
	{
		return CallInjectInputForAction(EnhancedInputComponent, EnhancedPlayerInput, "IA_Move", ETriggerEvent::Triggered, InputActionValueRight);
	}, 
	[this, World]()
	{
		TArray<AActor*> InventoryItems;
		UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
		TestTrueExpr(InventoryItems.Num() == 0);
		return true;
	}, 
	3.0f));
	
	return true;
}

#endif
