using UnrealBuildTool;

public class RoomTemplateDesigner : ModuleRules
{
    public RoomTemplateDesigner(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
             new string[]
             {
                "RoomTemplateDesigner/Public"
                 // ... add public include paths required here ...
             }
         );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "RoomTemplateDesigner/Private",
				// ... add other private include paths required here ...
			}
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "ALegendaryMine"
				// ... add other public dependencies that you statically link with here ...
			}
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Json",
                "Slate",
                "SlateCore",
                "EditorWidgets",
                "Engine",
                "InputCore",
                "UnrealEd", // for FAssetEditorManager
                "KismetWidgets",
                "Kismet",  // for FWorkflowCentricApplication
                "PropertyEditor",
                "RenderCore",
                "ContentBrowser",
                "WorkspaceMenuStructure",
                "EditorStyle",
                "MeshPaint",
                "EditorWidgets",
                "Projects",
                "AssetRegistry",
                "AdvancedPreviewScene",
                "EditorStyle",
                "RoomTemplateDesigner",
                "LevelEditor"
                // ... add private dependencies that you statically link with here ...	
	        }
        );

        PrivateIncludePathModuleNames.AddRange(
           new string[]
           {
                    "Settings",
                    "IntroTutorials",
                    "AssetTools",
                    "LevelEditor"
           }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                "AssetTools"
            }
        );
    }
}