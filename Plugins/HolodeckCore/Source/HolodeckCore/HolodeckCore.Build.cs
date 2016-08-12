// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;

public class HolodeckCore : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "..", "Binaries", "ThirdParty")); }
    }

    public HolodeckCore(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "Json", "JsonUtilities", "AIModule", "Projects"});

        PrivateDependencyModuleNames.AddRange(new string[] {  });

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

        LoadZMQ(Target);

    }

    // Deprecated in favor of FSocket
    public bool LoadZMQ(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        string LibraryPath = Path.Combine(ThirdPartyPath, "zeromq");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;
            string LibPath = Path.Combine(LibraryPath, "Win64", "libzmq.lib");
            string DLLPath = Path.Combine(LibraryPath, "Win64", "libzmq.dll");
            
            PublicAdditionalLibraries.Add(LibPath);
            PublicDelayLoadDLLs.Add("libzmq.dll");
            
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(LibraryPath, "Win64", "libzmq.dll")));
            
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            isLibrarySupported = true;
            string SOPath = Path.Combine(LibraryPath, "Linux", "libzmq.so");
            string SOPathVersion = Path.Combine(LibraryPath, "Linux", "libzmq.so.5");

            PublicAdditionalLibraries.Add(SOPath);
            RuntimeDependencies.Add(new RuntimeDependency(SOPath));
            RuntimeDependencies.Add(new RuntimeDependency(SOPathVersion));
        }


        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "zeromq", "Includes"));
        }

        Definitions.Add(string.Format("WITH_ZMQ_BINDING={0}", isLibrarySupported ? 1 : 0));
        Definitions.Add("ZMQ_STATIC=1");

        return true;
    }
}
