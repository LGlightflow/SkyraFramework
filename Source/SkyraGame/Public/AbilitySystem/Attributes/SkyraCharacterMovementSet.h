// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SkyraAttributeSet.h"
#include "SkyraCharacterMovementSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SKYRAGAME_API USkyraCharacterMovementSet : public USkyraAttributeSet
{
	GENERATED_BODY()
	
public:
	USkyraCharacterMovementSet();
	
	ATTRIBUTE_ACCESSORS(USkyraCharacterMovementSet, WalkSpeed);
	
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WalkSpeed, Category = "Skyra|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WalkSpeed;
	
	UFUNCTION()
	void OnRep_WalkSpeed(const FGameplayAttributeData& OldValue) const;
};
