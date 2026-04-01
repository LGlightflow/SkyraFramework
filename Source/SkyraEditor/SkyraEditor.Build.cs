// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SkyraEditor : ModuleRules
{
    public SkyraEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"SkyraEditor",
				"$(ModuleDir)/Public",
				"$(ModuleDir)/Public/AssetType",				
				"$(ModuleDir)/Public/Factory",			
				"$(ModuleDir)/Public/Inventory",						
				"$(ModuleDir)/Public/Style",			
				"$(ModuleDir)/Public/Validation",
				"$(ModuleDir)/Public/Commandlets",
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
				"$(ModuleDir)/Public",
				"$(ModuleDir)/Public/AssetType",				
				"$(ModuleDir)/Public/Factory",			
				"$(ModuleDir)/Public/Inventory",						
				"$(ModuleDir)/Public/Style",			
				"$(ModuleDir)/Public/Validation",
				"$(ModuleDir)/Public/Commandlets",
			}
		);

		PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "EditorFramework",
                "UnrealEd",
				"PhysicsCore",
				"GameplayTagsEditor",
				"GameplayTasksEditor",
				"GameplayAbilities",
				"GameplayAbilitiesEditor",
				"StudioTelemetry",
				"SkyraGame",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
				"InputCore",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"EditorStyle",
				"DataValidation",
				"MessageLog",
				"Projects",
				"DeveloperToolSettings",
				"CollectionManager",
				"SourceControl",
				"Chaos"
			}
        );

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
			}
		);

		// Generate compile errors if using DrawDebug functions in test/shipping builds.
		PublicDefinitions.Add("SHIPPING_DRAW_DEBUG_ERROR=1");
    }
}
