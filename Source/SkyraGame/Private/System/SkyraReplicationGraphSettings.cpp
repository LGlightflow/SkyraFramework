// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraReplicationGraphSettings.h"
#include "Misc/App.h"
#include "System/SkyraReplicationGraph.h"

USkyraReplicationGraphSettings::USkyraReplicationGraphSettings()
{
	CategoryName = TEXT("Game");
	DefaultReplicationGraphClass = USkyraReplicationGraph::StaticClass();
}