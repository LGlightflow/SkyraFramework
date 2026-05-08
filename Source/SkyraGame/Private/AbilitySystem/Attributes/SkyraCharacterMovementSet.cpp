// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/SkyraCharacterMovementSet.h"

#include "Net/UnrealNetwork.h"

USkyraCharacterMovementSet::USkyraCharacterMovementSet()
	:WalkSpeed(300.f)
{
}

void USkyraCharacterMovementSet::OnRep_WalkSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USkyraCharacterMovementSet, WalkSpeed, OldValue)
}

void USkyraCharacterMovementSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(USkyraCharacterMovementSet,WalkSpeed, COND_OwnerOnly, REPNOTIFY_Always);
	
}
