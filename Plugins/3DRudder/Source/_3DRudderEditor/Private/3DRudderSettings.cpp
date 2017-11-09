
#include "3DRudderSettings.h"
#include "3DRudderEditor.h"

U3DRudderSettings::U3DRudderSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bActive = true;
	Translation = FVector::OneVector;
	RotationYaw = 1.0f;
}