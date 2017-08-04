#include "3DRudderEditor.h"
 
// Settings
#include "3DRudderSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"

// Editor
#include "EditorViewportClient.h"
#include "Editor.h"

// 3dRudder
#define _3DRUDDER_SDK_STATIC
#include "3DRudderSDK.h"

IMPLEMENT_MODULE(F3DRudderEditorModule, 3DRudderEditor);

DEFINE_LOG_CATEGORY_STATIC(_3DRudderEditor, Log, All)
#define LOCTEXT_NAMESPACE "3DRudderEditor"
 
void F3DRudderEditorModule::StartupModule()
{
	UE_LOG(_3DRudderEditor, Warning, TEXT("3DRudderEditor: Log Started"));

	RegisterSettings();
}

void F3DRudderEditorModule::RegisterSettings()
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

void F3DRudderEditorModule::Tick(float DeltaTime)
{
	//UE_LOG(_3DRudderEditor, Warning, TEXT("tick %f"), DeltaTime);
	// 3dRudder SDK
	ns3dRudder::CSdk* pSdk = ns3dRudder::GetSDK();
	// Mode : curve
	ns3dRudder::ModeAxis mode = ns3dRudder::ValueWithCurveNonSymmetricalPitch;
	// Curves for each axis (Pitch, Roll, Yaw, UpDown)
	ns3dRudder::CurveArray *curves = new ns3dRudder::CurveArray;
	// Only one device (0)
	uint32 i = 0;
	if (pSdk->IsDeviceConnected(i))
	{
		// Axis : X, Y, Z, rZ
		ns3dRudder::Axis axis;
		// Status of 3dRudder
		ns3dRudder::Status status;
		if (pSdk->GetAxis(i, mode, &axis, curves) == ns3dRudder::Success)
		{
			status = pSdk->GetStatus(i);			
			if ((status == ns3dRudder::InUse || status == ns3dRudder::ExtendedMode) && GetDefault<U3DRudderSettings>()->bActive)
				UpdateViewportCamera(FVector(axis.m_aY, axis.m_aX, axis.m_aZ), axis.m_rZ);
		}
	}
}

void F3DRudderEditorModule::UpdateViewportCamera(const FVector& translation, float yaw)
{
	//UE_LOG(_3DRudderEditor, Warning, TEXT("tick %f"), yaw);
	if (translation.IsZero() && yaw == 0)
		return;

	if (GEditor != nullptr && GEditor->GetActiveViewport() != nullptr && GEditor->GetActiveViewport()->GetClient() != nullptr)
	{
		FEditorViewportClient* client = StaticCast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());		
		if (client != nullptr && !client->Viewport->IsPlayInEditorViewport())
		{			
			const FVector speed = GetDefault<U3DRudderSettings>()->Translation;
			const float speedRotation = GetDefault<U3DRudderSettings>()->RotationYaw;
			// X Y local
			FVector local(translation.X * speed.X, translation.Y * speed.Y, 0);
			FVector world = client->GetViewRotation().RotateVector(local);
			// Z world
			world += FVector(0.0f, 0.0f, translation.Z * speed.Z);
			// Pitch Yaw Roll
			FRotator rotation(0, yaw * speedRotation, 0);
			// Move Camera of Viewport with 3dRudder
			client->MoveViewportCamera(world, rotation);
		}
	}
}

void F3DRudderEditorModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "3dRudder", "General");
	}
}

void F3DRudderEditorModule::ShutdownModule()
{
	UE_LOG(_3DRudderEditor, Warning, TEXT("3DRudderEditor: Log Ended"));
	UnregisterSettings();
	ns3dRudder::EndSDK();
}
 
#undef LOCTEXT_NAMESPACE