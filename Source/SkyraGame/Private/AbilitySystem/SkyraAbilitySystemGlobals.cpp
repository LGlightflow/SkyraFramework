// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraAbilitySystemGlobals.h"

#include "SkyraGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraAbilitySystemGlobals)

struct FGameplayEffectContext;

USkyraAbilitySystemGlobals::USkyraAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* USkyraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSkyraGameplayEffectContext();
}

