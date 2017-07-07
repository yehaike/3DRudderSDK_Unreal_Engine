// Copyright 3dRudder 2017, Inc. All Rights Reserved.

#include "3DRudderPrivatePCH.h"
 
#include "3DRudderDevice.h"
 
#include "SlateBasics.h"
 
#include "WindowsApplication.h"
#include "WindowsWindow.h"
#include "WindowsCursor.h"
#include "GenericApplicationMessageHandler.h"
#include "InputDevice.h"
#include "EditorViewportClient.h"
#include "Editor.h"
#include "3DRudderSettings.h"

#define _3DRUDDER_SDK_STATIC
#include "3DRudderSDK.h"

#define LOCTEXT_NAMESPACE "3dRudder"
 
const FKey EKeys3dRudder::XAxis("XAxis");
const FKey EKeys3dRudder::YAxis("YAxis");
const FKey EKeys3dRudder::ZAxis("ZAxis");
const FKey EKeys3dRudder::ZRotation("ZRotation");
const FKey EKeys3dRudder::Status("Status");
const FKey EKeys3dRudder::Sensor1("Sensor1");
const FKey EKeys3dRudder::Sensor2("Sensor2");
const FKey EKeys3dRudder::Sensor3("Sensor3");
const FKey EKeys3dRudder::Sensor4("Sensor4");
const FKey EKeys3dRudder::Sensor5("Sensor5");
const FKey EKeys3dRudder::Sensor6("Sensor6");

DEFINE_LOG_CATEGORY_STATIC(Log3dRudderDevice, Log, All);

//UE v4.6 IM event wrappers
bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int32 user, bool repeat)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

F3DRudderDevice::F3DRudderDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
	: m_MessageHandler(InMessageHandler)
{
	UE_LOG(Log3dRudderDevice, Log, TEXT("3dRudder version %x"), ns3dRudder::GetSDK()->GetSDKVersion()); 
 
	// Register the FKeys (Gamepad key for controllers)
	EKeys::AddMenuCategoryDisplayInfo("3dRudder", LOCTEXT("3dRudderSubCateogry", "3dRudder"), TEXT("GraphEditor.PadEvent_16x"));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::XAxis, LOCTEXT("XAxis", "3dRudder XAxis"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::YAxis, LOCTEXT("YAxis", "3dRudder YAxis"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::ZAxis, LOCTEXT("ZAxis", "3dRudder ZAxis"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::ZRotation, LOCTEXT("ZRotation", "3dRudder ZRotation"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Status, LOCTEXT("Status", "3dRudder Status"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor1, LOCTEXT("Sensor1", "3dRudder Sensor1"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor2, LOCTEXT("Sensor2", "3dRudder Sensor2"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor3, LOCTEXT("Sensor3", "3dRudder Sensor3"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor4, LOCTEXT("Sensor4", "3dRudder Sensor4"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor5, LOCTEXT("Sensor5", "3dRudder Sensor5"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));
	EKeys::AddKey(FKeyDetails(EKeys3dRudder::Sensor6, LOCTEXT("Sensor6", "3dRudder Sensor6"), FKeyDetails::FloatAxis, FName(TEXT("3dRudder"))));

}

void F3DRudderDevice::Tick(float DeltaTime)
{
	// This will spam the log heavily, comment it out for real plugins :)
	// UE_LOG(Log3dRudderDevice, Log, TEXT("Tick %f"), DeltaTime);
}
 
void F3DRudderDevice::SendControllerEvents() 
{
	// 3dRudder SDK
	ns3dRudder::CSdk* pSdk = ns3dRudder::GetSDK();
	// Mode : curve
	ns3dRudder::ModeAxis mode = (ns3dRudder::ModeAxis)GetDefault<U3DRudderSettings>()->Mode;
	// Curves for each axis (Pitch, Roll, Yaw, UpDown)
	ns3dRudder::CurveArray *curves = new ns3dRudder::CurveArray;
	// For each device (4)
	for (unsigned int i = 0;i < _3DRUDDER_SDK_MAX_DEVICE;i++)
	{
		if (pSdk->IsDeviceConnected(i))
		{
			// Axis : X, Y, Z, rZ
			ns3dRudder::Axis axis;
			// Status of 3dRudder
			ns3dRudder::Status status;
			if (pSdk->GetAxis(i, mode, &axis, curves) == ns3dRudder::Success)
			{
				status = pSdk->GetStatus(i);
				// Roll
				EmitAnalogInputEventForKey(EKeys3dRudder::XAxis, axis.m_aX, i, 0);
				// Pitch				   
				EmitAnalogInputEventForKey(EKeys3dRudder::YAxis, axis.m_aY, i, 0);
				// UpDown				   
				EmitAnalogInputEventForKey(EKeys3dRudder::ZAxis, axis.m_aZ, i, 0);
				// Yaw					   
				EmitAnalogInputEventForKey(EKeys3dRudder::ZRotation, axis.m_rZ, i, 0);
				// Status				   
				EmitAnalogInputEventForKey(EKeys3dRudder::Status, status, i, 0);
				// 6 Sensors			   
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor1, pSdk->GetSensor(i, 0), i, 0);
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor2, pSdk->GetSensor(i, 1), i, 0);
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor3, pSdk->GetSensor(i, 2), i, 0);
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor4, pSdk->GetSensor(i, 3), i, 0);
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor5, pSdk->GetSensor(i, 4), i, 0);
				EmitAnalogInputEventForKey(EKeys3dRudder::Sensor6, pSdk->GetSensor(i, 5), i, 0);				

				if (i == 0 && GetDefault<U3DRudderSettings>()->bActive)
					UpdateViewportCamera(FVector(axis.m_aY, axis.m_aX, axis.m_aZ), axis.m_rZ);
			}							   
		}
	}
}
 
void F3DRudderDevice::UpdateViewportCamera(const FVector& translation, float yaw)
{
	if (translation.IsZero() && yaw == 0)
		return;

	if (GEditor != nullptr && GEditor->GetActiveViewport() != nullptr && GEditor->GetActiveViewport()->GetClient() != nullptr)
	{
		FEditorViewportClient* client = StaticCast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
		if (client != nullptr)
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

void F3DRudderDevice::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) 
{
	UE_LOG(Log3dRudderDevice, Log, TEXT("Set Message Handler"));
	m_MessageHandler = InMessageHandler;
}
 
bool F3DRudderDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) 
{
	UE_LOG(Log3dRudderDevice, Log, TEXT("Execute Console Command: %s"), Cmd);
 
	// Put your fancy custom console command code here... 
	// ToDo: use this to let you fire pseudo controller events
 
	return true;
}

// IForceFeedbackSystem pass through functions
void F3DRudderDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) 
{
	UE_LOG(Log3dRudderDevice, Log, TEXT("Set Force Feedback %f"), Value);
}

void F3DRudderDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) 
{
	// This will spam the log heavily, comment it out for real plugins :)
	UE_LOG(Log3dRudderDevice, Log, TEXT("Set Force Feedback Values"));
} 

// This is where you nicely clean up your plugin when its told to shut down!
F3DRudderDevice::~F3DRudderDevice() 
{
	ns3dRudder::EndSDK();
	UE_LOG(Log3dRudderDevice, Log, TEXT("Closing 3DRudderDevice"));
}