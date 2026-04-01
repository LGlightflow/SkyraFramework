// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Character/SkyraCharacter.h"

#include "SkyraCharacterWithAbilities.generated.h"

class UAbilitySystemComponent;
class USkyraAbilitySystemComponent;
class UObject;

// ASkyraCharacter typically gets the ability system component from the possessing player state
// This represents a character with a self-contained ability system component.
UCLASS(Blueprintable)
class SKYRAGAME_API ASkyraCharacterWithAbilities : public ASkyraCharacter
{
	GENERATED_BODY()

public:
	ASkyraCharacterWithAbilities(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Skyra|PlayerState")
	TObjectPtr<USkyraAbilitySystemComponent> AbilitySystemComponent;
	
	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class USkyraHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class USkyraCombatSet> CombatSet;
};
