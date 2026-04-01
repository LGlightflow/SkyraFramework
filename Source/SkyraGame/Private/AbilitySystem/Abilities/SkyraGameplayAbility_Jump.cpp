// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraGameplayAbility_Jump.h"

#include "AbilitySystem/Abilities/SkyraGameplayAbility.h"
#include "Character/SkyraCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraGameplayAbility_Jump)

struct FGameplayTagContainer;


USkyraGameplayAbility_Jump::USkyraGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool USkyraGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
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

void USkyraGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USkyraGameplayAbility_Jump::CharacterJumpStart()
{
	if (ASkyraCharacter* SkyraCharacter = GetSkyraCharacterFromActorInfo())
	{
		if (SkyraCharacter->IsLocallyControlled() && !SkyraCharacter->bPressedJump)
		{
			SkyraCharacter->UnCrouch();
			SkyraCharacter->Jump();
		}
	}
}

void USkyraGameplayAbility_Jump::CharacterJumpStop()
{
	if (ASkyraCharacter* SkyraCharacter = GetSkyraCharacterFromActorInfo())
	{
		if (SkyraCharacter->IsLocallyControlled() && SkyraCharacter->bPressedJump)
		{
			SkyraCharacter->StopJumping();
		}
	}
}

