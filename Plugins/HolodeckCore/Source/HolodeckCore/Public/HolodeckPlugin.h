// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHolodeck, All, All);

/**
 * The public interface to the Leap Motion Controller plugin module.
 */
class FHolodeckPlugin : public IModuleInterface
{

public:

	void* LibHandle;

	// IModuleInterface implementation.
	virtual void StartupModule();
	virtual void ShutdownModule();

	/**
	 * Singleton-like access to this module's interface.
	 * Beware of calling this during the shutdown phase.  The module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FHolodeckPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< FHolodeckPlugin >( "HolodeckCore" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "HolodeckCore" );
	}

};

