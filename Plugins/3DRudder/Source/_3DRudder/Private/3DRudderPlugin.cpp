// Copyright 3dRudder 2017, Inc. All Rights Reserved.

#include "3DRudderPlugin.h" 
#include "3DRudderPrivatePCH.h"

#include "Internationalization.h" // LOCTEXT
#include "InputCoreTypes.h"
 
// Plugins
#include "I3DRudderPlugin.h"

IMPLEMENT_MODULE(F3DRudderPlugin, _3DRudder)
DEFINE_LOG_CATEGORY_STATIC(_3dRudderPlugin, Log, All); 
 
// This function is called by *Application.cpp after startup to instantiate the modules InputDevice
TSharedPtr< class IInputDevice > F3DRudderPlugin::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	UE_LOG(_3dRudderPlugin, Log, TEXT("Create Input Device"));

	m_3DRudderDevice = MakeShareable(new F3DRudderDevice(InMessageHandler));	

	return m_3DRudderDevice;
}

// This function may be called during shutdown to clean up the module.
void F3DRudderPlugin::ShutdownModule()
{
	m_3DRudderDevice->~F3DRudderDevice();

	UE_LOG(_3dRudderPlugin, Log, TEXT("Shutdown Module"));
}