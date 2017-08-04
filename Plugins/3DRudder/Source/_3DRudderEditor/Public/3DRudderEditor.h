#pragma once
 
#include "Engine.h"
#include "ModuleManager.h"
#include "UnrealEd.h"
 
class F3DRudderEditorModule: public IModuleInterface, public FTickableEditorObject
{
public:
	// Module
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// Settings
	void RegisterSettings();
	void UnregisterSettings(); 

	// Tickable
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const
	{
		return true;
	}
	virtual TStatId GetStatId() const
	{
		return TStatId();
	};

	// Editor
	void UpdateViewportCamera(const FVector& translation, float yaw);
};