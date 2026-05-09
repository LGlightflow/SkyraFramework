// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraCombatSet.h"

#include "AbilitySystem/Attributes/SkyraAttributeSet.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraCombatSet)

class FLifetimeProperty;


USkyraCombatSet::USkyraCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
	, IncomingDamageMultiplier(1.f)
	, IncomingHealingMultiplier(1.f)
	,OutgoingDamageMultiplier(1.0f)
	,OutgoingHealingMultiplier(1.0f)
	,CriticalRate(0.f)
	,CriticalDamageMultiplier(1.f)
	, Armor(1.f)
{
}

void USkyraCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, IncomingDamageMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, IncomingHealingMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, Armor, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, CriticalRate, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCombatSet, CriticalDamageMultiplier, COND_OwnerOnly, REPNOTIFY_Always);
}

void USkyraCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, BaseDamage, OldValue);
}

void USkyraCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, BaseHeal, OldValue);
}

void USkyraCombatSet::OnRep_IncomingDamageMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, IncomingDamageMultiplier, OldValue);
}

void USkyraCombatSet::OnRep_IncomingHealingMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, IncomingHealingMultiplier, OldValue);
}

void USkyraCombatSet::OnRep_CriticalRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, CriticalRate, OldValue);
}


void USkyraCombatSet::OnRep_CriticalDamageMultiplier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, CriticalDamageMultiplier, OldValue);
}

void USkyraCombatSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCombatSet, Armor, OldValue);
}



