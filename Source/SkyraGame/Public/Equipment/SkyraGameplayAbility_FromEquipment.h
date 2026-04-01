// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/SkyraGameplayAbility.h"

#include "SkyraGameplayAbility_FromEquipment.generated.h"

class USkyraEquipmentInstance;
class USkyraInventoryItemInstance;

/**
 * USkyraGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class USkyraGameplayAbility_FromEquipment : public USkyraGameplayAbility
{
	GENERATED_BODY()

public:

	USkyraGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Skyra|Ability")
	USkyraEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Skyra|Ability")
	USkyraInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
