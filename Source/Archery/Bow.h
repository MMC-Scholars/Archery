

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Components/SceneComponent.h"
#include "LineTools/linetools.h"
#include "Arrow.h"
#include "Bow.generated.h"

UCLASS()
class ARCHERY_API ABow : public APickup {
	GENERATED_BODY()

public:
	// Constructor
	ABow();

	// PreInit
	virtual void PreInit() override;

	// DefaultThink
	virtual void DefaultThink() override;
	
	// Components
	USceneComponent* m_pStringTop;
	USceneComponent* m_pStringBot;
	USceneComponent* m_pStringMid;

	// Variables

	// reset
	FVector m_initLoc;
	FRotator m_initRot;
	// arrow
	AArrow* m_pNotchedArrow;
	//TODO handle
	//EHANDLE m_hNotchedArrow;
	float m_fArrowVelocity;
	// string
	SLineDrawParams m_sStringProps;
	// haptic
	bool m_bHapticPulse;
	float m_fHapticPulseTime;
	
	// Functions

	// ArrowNotch
	void ArrowNotch(AArrow* arrow);

	// Pickup
	virtual void OnPickup_Implementation(ABaseController* controller) override;
	// Drop
	virtual void OnDrop_Implementation(ABaseController* controller) override;
};
