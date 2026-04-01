// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraWeaponDebugSettings.h"
#include "Misc/App.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraWeaponDebugSettings)

USkyraWeaponDebugSettings::USkyraWeaponDebugSettings()
{
}

FName USkyraWeaponDebugSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}

