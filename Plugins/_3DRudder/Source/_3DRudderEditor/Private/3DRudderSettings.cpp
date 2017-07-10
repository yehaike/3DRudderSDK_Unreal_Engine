#include "3DRudderEditor.h"
#include "3DRudderSettings.h"

U3DRudderSettings::U3DRudderSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActive = true;
	Translation = FVector::OneVector;
	RotationYaw = 1.0f;
}