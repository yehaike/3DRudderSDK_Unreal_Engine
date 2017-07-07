#include "3DRudderPrivatePCH.h"

#include "3DRudderSettings.h"

U3DRudderSettings::U3DRudderSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActive = true;
	Mode = E3DRudderModeAxis::ValueWithCurveNonSymmetricalPitch;
	Translation = FVector::OneVector;
	RotationYaw = 1.0f;
}