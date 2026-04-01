// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemGlobals.h"

#include "SkyraAbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

UCLASS(Config=Game)
class USkyraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_UCLASS_BODY()

	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
