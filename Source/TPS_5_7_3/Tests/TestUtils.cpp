
#if WITH_AUTOMATION_TESTS

#include "TPS_5_7_3/Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"

namespace TPS
{
	namespace Test
	{
	UWorld* GetTestGameWorld()
	{
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for ( const FWorldContext& Context : WorldContexts )
		{
			if (( Context.WorldType == EWorldType::PIE  ||  Context.WorldType == EWorldType::Game  ) &&  Context.World() != nullptr )
			{
				return Context.World();
			}
		}

		return nullptr;
	}
		
	void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
	{	
		if (!Object) return;
		
		FString Command = FString::Printf(TEXT("%s"), *FuncName);
		for (const auto Param : Params)
		{
			Command.Append(" ").Append(Param);
		}
		FOutputDeviceNull OutputDeviceNull;
		Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
	}
		
	int32 GetActionBindingIndexByName(const UEnhancedInputComponent* InputComponent, const FString& ActionName, ETriggerEvent TriggerEvent)
	{
		if (!IsValid(InputComponent)) return INDEX_NONE;
		for (int32 i = 0; i < InputComponent->GetActionEventBindings().Num(); ++i)
		{
			const auto& Binding = InputComponent->GetActionEventBindings()[i];
			if (Binding->GetAction()->GetName().Equals(ActionName) && Binding->GetTriggerEvent() == TriggerEvent)
			{
				return i;
			}
		}
		return INDEX_NONE;
	}
	
	bool CallInjectInputForAction(const UEnhancedInputComponent* EnhancedInputComponent, UEnhancedPlayerInput* EnhancedPlayerInput, 
		const FString& ActionName, ETriggerEvent TriggerEvent, const FInputActionValue& InputActionValue)
	{
		const int32 MoveActionIndex =  GetActionBindingIndexByName(EnhancedInputComponent, ActionName, TriggerEvent);
		if (MoveActionIndex == INDEX_NONE) return true;
		const UInputAction* InputAction = EnhancedInputComponent->GetActionEventBindings()[MoveActionIndex]->GetAction();
		
		EnhancedPlayerInput->InjectInputForAction(InputAction, InputActionValue);
		return false;
	}
		
	}
}

#endif