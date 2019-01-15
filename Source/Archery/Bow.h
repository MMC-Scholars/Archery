

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Arrow.h"
#include "Bow.generated.h"

UCLASS()
class ARCHERY_API ABow : public APickup {
	GENERATED_BODY()

public:
	// PreInit
	virtual void PreInit() override;

	// DefaultThink
	virtual void DefaultThink() override;
	
	// Variables
	AArrow* m_pNotchedArrow;
	float m_fArrowVelocity;
	//short int m_iHapticFac;

	// ArrowNotch
	void ArrowNotch(AArrow* arrow);

	// Pickup
	virtual void OnPickup_Implementation(ABaseController* controller) override;
	// Drop
	virtual void OnDrop_Implementation(ABaseController* controller) override;
};
