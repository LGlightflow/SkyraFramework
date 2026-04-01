// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Inventory/SkyraInventoryItemDefinition.h"

#include "InventoryFragment_ReticleConfig.generated.h"

class USkyraReticleWidgetBase;
class UObject;

UCLASS()
class UInventoryFragment_ReticleConfig : public USkyraInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<USkyraReticleWidgetBase>> ReticleWidgets;
};
