// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "SkyraWeaponUserInterface.generated.h"

class USkyraWeaponInstance;
class UObject;
struct FGeometry;

UCLASS()
class USkyraWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	USkyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(USkyraWeaponInstance* OldWeapon, USkyraWeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<USkyraWeaponInstance> CurrentInstance;
};
