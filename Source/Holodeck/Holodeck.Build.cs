// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Holodeck : ModuleRules
{
	public Holodeck(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateIncludePaths.AddRange(new string[] {
             "Holodeck/Agents/Public",
             "Holodeck/General/Public",
             "Holodeck/Sensors/Public",
             "Holodeck/Utils/Public",
             "Holodeck/HolodeckCore/Public",
             "Holodeck/ClientCommands/Public"
         });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "SlateCore", "Slate", "PhysX", "APEX", "JsonUtilities", "RenderCore", "RHI" });

#if PLATFORM_LINUX
        PublicDependencyModuleNames.AddRange(new string[] { "rt", "pthread" };
        //TARGET_LINK_LIBRARIES(UHolodeckServer rt pthread)
#endif

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }
}
