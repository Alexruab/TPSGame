#pragma once

#if WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"
#include "InputTriggers.h"

namespace TPS
{
	
namespace Test
{
	
#define ENUM_LOOP_START(TYPE, EnumElem) \
{ \
const UEnum* InvEnum = StaticEnum<TYPE>(); \
ensure(InvEnum); \
for (int32 index = 0; index < InvEnum->NumEnums() - 1; ++index) \
{ \
	const auto EnumElem = static_cast<TYPE>(index); \
	
#define ENUM_LOOP_END }}

	template<typename T>
	T* CreateBlueprint(UWorld* World, const FString& BPName, const FTransform& Transform = FTransform::Identity)
	{
		const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BPName);

		return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
	}
	
	template<typename T>
	T* CreateBlueprintDeferred(UWorld* World, const FString& BPName, const FTransform& Transform = FTransform::Identity)
	{
		const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BPName);

		return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
	}
	
	class LevelScope
	{
	public:
		LevelScope(const FString& MapName) { AutomationOpenMap(MapName); }
		
		~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
	};
	
	UWorld* GetTestGameWorld();
	
	void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);
	
	int32 GetActionBindingIndexByName(const UEnhancedInputComponent* InputComponent, const FString& ActionName, ETriggerEvent TriggerEvent);
	
	
	bool CallInjectInputForAction(const UEnhancedInputComponent* EnhancedInputComponent, UEnhancedPlayerInput* EnhancedPlayerInput, 
		const FString& ActionName, ETriggerEvent TriggerEvent, const FInputActionValue& InputActionValue);
	
	
}
} 

#endif