// Copyright Epic Games, Inc. All Rights Reserved.

#include "Abilities/SkyraGameplayAbility_CharJump.h"

#include "AbilitySystem/Abilities/SkyraGameplayAbility.h"
#include "Character/SkyraCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraGameplayAbility_CharJump)

struct FGameplayTagContainer;


USkyraGameplayAbility_CharJump::USkyraGameplayAbility_CharJump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool USkyraGameplayAbility_CharJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ASkyraCharacter* SkyraCharacter = Cast<ASkyraCharacter>(ActorInfo->AvatarActor.Get());
	if (!SkyraCharacter || !SkyraCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void USkyraGameplayAbility_CharJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USkyraGameplayAbility_CharJump::CharacterJumpStart()
{
	if (ASkyraCharacter* SkyraCharacter = (CurrentActorInfo ? Cast<ASkyraCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr))
	{
		if (SkyraCharacter->IsLocallyControlled() && !SkyraCharacter->bPressedJump)
		{
			SkyraCharacter->UnCrouch();
			SkyraCharacter->Jump();
		}
	}
}

void USkyraGameplayAbility_CharJump::CharacterJumpStop()
{
	if (ASkyraCharacter* SkyraCharacter = (CurrentActorInfo ? Cast<ASkyraCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr))
	{
		if (SkyraCharacter->IsLocallyControlled() && SkyraCharacter->bPressedJump)
		{
			SkyraCharacter->StopJumping();
		}
	}
}

