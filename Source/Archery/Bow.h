

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
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

	// UProperty to hide help text as needed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Help Text")
	ATextRenderActor* m_pHelpText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Help Text 2")
	ATextRenderActor* m_pHelpText2;

	// string
	SLineDrawParams m_sStringProps;
	FVector m_vInitStringLoc;
	// arrow
	AArrow* m_pNotchedArrow;
	// arrow velocity
	float m_fArrowVelocity;
	// haptic
	bool m_bHapticPulse;
	float m_fHapticPulseTime;

	// Functions

	// Bow Reset
	void ResetBow();
	
	// Arrow Notch
	void ArrowNotch(AArrow* arrow);

	// Pickup
	// Note: this function is OVERRIDEN from the main Pickup
	// function, NOT an implementation of OnPickup.
	virtual void Pickup(ABaseController* controller) override;
	
	// OnDrop Implementation
	virtual void OnDrop_Implementation(ABaseController* controller) override;
};
