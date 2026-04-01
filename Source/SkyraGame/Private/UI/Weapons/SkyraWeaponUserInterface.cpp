// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraWeaponUserInterface.h"

#include "Equipment/SkyraEquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/SkyraWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraWeaponUserInterface)

struct FGeometry;

USkyraWeaponUserInterface::USkyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USkyraWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkyraWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (USkyraEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<USkyraEquipmentManagerComponent>())
		{
			if (USkyraWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<USkyraWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					USkyraWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void USkyraWeaponUserInterface::RebuildWidgetFromWeapon()
{
	
}

