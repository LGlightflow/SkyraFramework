// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/SkyraGameplayAbility.h"

#include "SkyraGameplayAbility_Reset.generated.h"

class AActor;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

/**
 * USkyraGameplayAbility_Reset
 *
 *	Gameplay ability used for handling quickly resetting the player back to initial spawn state.
 *	Ability is activated automatically via the "GameplayEvent.RequestReset" ability trigger tag (server only).
 */
UCLASS()
class SKYRAGAME_API USkyraGameplayAbility_Reset : public USkyraGameplayAbility
{
	GENERATED_BODY()

public:
	USkyraGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};



USTRUCT(BlueprintType)
struct FSkyraPlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};
