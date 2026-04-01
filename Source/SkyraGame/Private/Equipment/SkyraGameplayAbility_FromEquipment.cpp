// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraGameplayAbility_FromEquipment.h"
#include "SkyraEquipmentInstance.h"
#include "Inventory/SkyraInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraGameplayAbility_FromEquipment)

USkyraGameplayAbility_FromEquipment::USkyraGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USkyraEquipmentInstance* USkyraGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<USkyraEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

USkyraInventoryItemInstance* USkyraGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (USkyraEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<USkyraInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult USkyraGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Context.AddError(NSLOCTEXT("Skyra", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
