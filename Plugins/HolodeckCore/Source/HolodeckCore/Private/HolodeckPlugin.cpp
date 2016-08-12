// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "HolodeckPrivatePCH.h"
#include "IPluginManager.h"
#include "HolodeckPlugin.h"

IMPLEMENT_MODULE(FHolodeckPlugin, HolodeckCore)

DEFINE_LOG_CATEGORY(LogHolodeck)

void FHolodeckPlugin::StartupModule()
{
	const FString BaseDir = IPluginManager::Get().FindPlugin("HolodeckCore")->GetBaseDir();
	const FString LibzmqDir = FPaths::Combine(*BaseDir, TEXT("Binaries"), TEXT("ThirdParty"), TEXT("zeromq"));
	
	#if PLATFORM_WINDOWS
		#if PLATFORM_64BITS
		
			const FString DLLPath = FPaths::Combine(*LibzmqDir, TEXT("Win64"), TEXT("libzmq.dll"));
			const FString SodiumPath = FPaths::Combine(*LibzmqDir, TEXT("Win64"), TEXT("libsodium.dll"));

			LibHandle = FPlatformProcess::GetDllHandle(*DLLPath);

			//FPlatformProcess::GetDllHandle(*SodiumPath);

			if (LibHandle == nullptr) {
				GLog->Logf(ELogVerbosity::Warning, TEXT("HolodeckCore: Failed to load required library %s. Plug-in will not be functional."), *DLLPath);
			}

		#else
			// TODO: Implement 32bits
			GLog->Logf(ELogVerbosity::Fatal, TEXT("HolodeckCore: Platform (32bits) not supported. Plug-in will not be functional."));
		#endif

	#elif PLATFORM_LINUX
		const FString SOPath = FPaths::Combine(*LibzmqDir, TEXT("Linux"), TEXT("libzmq.so"));

		LibHandle = FPlatformProcess::GetDllHandle(*SOPath);

		if (LibHandle == nullptr) {
			GLog->Logf(ELogVerbosity::Warning, TEXT("HolodeckCore: Failed to load required library %s. Plug-in will not be functional."), *SOPath);
		}
	#endif

}

void FHolodeckPlugin::ShutdownModule()
{
	FPlatformProcess::FreeDllHandle(LibHandle);
}