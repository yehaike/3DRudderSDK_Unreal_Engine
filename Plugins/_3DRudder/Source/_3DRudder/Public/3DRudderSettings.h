#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "3DRudderSettings.generated.h"

UENUM()
enum class E3DRudderModeAxis : uint8
{
	Angle = 0,
	NormalizedValue = 2,
	ValueWithCurve = 3,
	NormalizedValueNonSymmetricalPitch = 4,
	ValueWithCurveNonSymmetricalPitch = 5
};

/**
* Setting object used to hold both config settings and editable ones in one place
* To ensure the settings are saved to the specified config file make sure to add
* props using the globalconfig or config meta.
*/
UCLASS(config = Editor, defaultconfig)
class U3DRudderSettings : public UObject
{
	GENERATED_BODY()

public:

	U3DRudderSettings(const FObjectInitializer& ObjectInitializer);

	/** Enable/Disable */
	UPROPERTY(EditAnywhere, config, Category = Move)
		bool bActive;

	/** Mode axis */
	UPROPERTY(EditAnywhere, config, Category = Move)
		E3DRudderModeAxis Mode;

	/** Speed Translation */
	UPROPERTY(EditAnywhere, config, Category = Speed)
		FVector Translation;

	/** Speed Rotation (Yaw) */
	UPROPERTY(EditAnywhere, config, Category = Speed, meta = (DisplayName = "Rotation (Yaw)" ))
		float RotationYaw;
};