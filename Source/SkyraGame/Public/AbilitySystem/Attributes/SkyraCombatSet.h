// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "SkyraAttributeSet.h"

#include "SkyraCombatSet.generated.h"

class UObject;
struct FFrame;


/**
 * USkyraCombatSet
 *
 *  Class that defines attributes that are necessary for applying damage or healing.
 *	Attribute examples include: damage, healing, attack power, and shield penetrations.
 */
UCLASS(BlueprintType)
class USkyraCombatSet : public USkyraAttributeSet
{
	GENERATED_BODY()

public:

	USkyraCombatSet();

	ATTRIBUTE_ACCESSORS(USkyraCombatSet, BaseDamage);
	ATTRIBUTE_ACCESSORS(USkyraCombatSet, BaseHeal);
	ATTRIBUTE_ACCESSORS(USkyraCombatSet, IncomingDamageMultiplier);
	ATTRIBUTE_ACCESSORS(USkyraCombatSet, IncomingHealingMultiplier);
	ATTRIBUTE_ACCESSORS(USkyraCombatSet, Armor);
protected:

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_IncomingDamageMultiplier(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_IncomingHealingMultiplier(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_CriticalRate(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_CriticalDamageMultiplier(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);
	
private:

	// The base amount of damage to apply in the damage execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseDamage, Category = "Skyra|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	// The base amount of healing to apply in the heal execution.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BaseHeal, Category = "Skyra|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;
	
	// Damage multiplier to me
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IncomingDamageMultiplier, Category="Skyra|Combat",Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData IncomingDamageMultiplier;

	// Healing multiplier to me
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IncomingHealingMultiplier, Category="Skyra|Combat",Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData IncomingHealingMultiplier;
	
	//damage multiplier from me
	UPROPERTY(BlueprintReadOnly,Category="Skyra|Combat", Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData OutgoingDamageMultiplier;
	
	// Healing multiplier from me
	UPROPERTY(BlueprintReadOnly, Category="Skyra|Combat", Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData OutgoingHealingMultiplier;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalRate, Category="Skyra|Combat",Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData CriticalRate;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalDamageMultiplier, Category="Skyra|Combat",Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData CriticalDamageMultiplier;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Skyra|Combat",Meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Armor;
	
	
	
};
