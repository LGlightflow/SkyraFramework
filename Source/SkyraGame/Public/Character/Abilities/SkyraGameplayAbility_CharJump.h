// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SkyraGameplayAbility.h"

#include "SkyraGameplayAbility_CharJump.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;


/**
 * USkyraGameplayAbility_CharJump
 *
 *	Gameplay ability used for character jumping.
 */
UCLASS(Abstract)
class USkyraGameplayAbility_CharJump : public USkyraGameplayAbility
{
	GENERATED_BODY()

public:

	USkyraGameplayAbility_CharJump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Skyra|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Skyra|Ability")
	void CharacterJumpStop();
};
