// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/SkyraGameplayAbility_Reset.h"

#include "AbilitySystem/SkyraAbilitySystemComponent.h"
#include "Character/SkyraCharacter.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "SkyraGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraGameplayAbility_Reset)

USkyraGameplayAbility_Reset::USkyraGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = SkyraGameplayTags::GameplayEvent_RequestReset;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void USkyraGameplayAbility_Reset::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	USkyraAbilitySystemComponent* SkyraASC = CastChecked<USkyraAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(SkyraGameplayTags::Ability_Behavior_SurvivesDeath);

	// Cancel all abilities and block others from starting.
	SkyraASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);

	SetCanBeCanceled(false);

	// Execute the reset from the character
	if (ASkyraCharacter* SkyraChar = Cast<ASkyraCharacter>(CurrentActorInfo->AvatarActor.Get()))
	{
		SkyraChar->Reset();
	}

	// Let others know a reset has occurred
	FSkyraPlayerResetMessage Message;
	Message.OwnerPlayerState = CurrentActorInfo->OwnerActor.Get();
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(SkyraGameplayTags::GameplayEvent_Reset, Message);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bReplicateEndAbility = true;
	const bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}

