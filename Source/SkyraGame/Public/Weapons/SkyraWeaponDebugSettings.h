// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "SkyraWeaponDebugSettings.generated.h"

class UObject;

/**
 * Developer debugging settings for weapons
 */
UCLASS(config=EditorPerProjectUserSettings)
class USkyraWeaponDebugSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	USkyraWeaponDebugSettings();

	//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override;
	//~End of UDeveloperSettings interface

public:
	// Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds)
	UPROPERTY(config, EditAnywhere, Category=General, meta=(ConsoleVariable="skyra.Weapon.DrawBulletTraceDuration", ForceUnits=s))
	float DrawBulletTraceDuration;

	// Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds)
	UPROPERTY(config, EditAnywhere, Category = General, meta = (ConsoleVariable = "skyra.Weapon.DrawBulletHitDuration", ForceUnits = s))
	float DrawBulletHitDuration;

	// When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in cm)
	UPROPERTY(config, EditAnywhere, Category = General, meta = (ConsoleVariable = "skyra.Weapon.DrawBulletHitRadius", ForceUnits=cm))
	float DrawBulletHitRadius;
};
