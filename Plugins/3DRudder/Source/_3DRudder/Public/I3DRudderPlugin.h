// Copyright 3dRudder 2017, Inc. All Rights Reserved.

#pragma once

#include <ModuleManager.h>
#include <IInputDeviceModule.h>


/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules 
 * within this plugin.
 */
class I3DRudderPlugin : public IInputDeviceModule
{
public:
	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline I3DRudderPlugin& Get() {
		return FModuleManager::LoadModuleChecked< I3DRudderPlugin >("3DRudder");
	}
 
	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded( "3DRudder" );
	}
};