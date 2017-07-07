// Copyright 3dRudder 2017, Inc. All Rights Reserved.

#include "3DRudderPrivatePCH.h"

#include "Internationalization.h" // LOCTEXT
#include "InputCoreTypes.h"
 
// Plugins
#include "3DRudderPlugin.h" 
#include "I3DRudderPlugin.h"

// Settings
#include "3DRudderSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"

IMPLEMENT_MODULE(F3DRudderPlugin, _3DRudder)
DEFINE_LOG_CATEGORY_STATIC(_3dRudderPlugin, Log, All); 
 
// This function is called by *Application.cpp after startup to instantiate the modules InputDevice
TSharedPtr< class IInputDevice > F3DRudderPlugin::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	UE_LOG(_3dRudderPlugin, Log, TEXT("Create Input Device"));
 
	RegisterSettings();

	m_3DRudderDevice = MakeShareable(new F3DRudderDevice(InMessageHandler));	

	return m_3DRudderDevice;
}

void F3DRudderPlugin::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Editor");

		SettingsContainer->DescribeCategory("3dRudder",
			LOCTEXT("RuntimeWDCategoryName", "3dRudder"),
			LOCTEXT("RuntimeWDCategoryDescription", "Editor configuration for the Plugin 3dRudder module"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Editor", "3dRudder", "Viewport",
			LOCTEXT("RuntimeGeneralSettingsName", "Viewport"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Move the camera in viewport with the 3dRudder controller"),
			GetMutableDefault<U3DRudderSettings>()
		);
	}
}

void F3DRudderPlugin::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "3dRudder", "General");
	}
}

// This function may be called during shutdown to clean up the module.
void F3DRudderPlugin::ShutdownModule()
{
	m_3DRudderDevice->~F3DRudderDevice();
 
	UnregisterSettings();

	UE_LOG(_3dRudderPlugin, Log, TEXT("Shutdown Module"));
}