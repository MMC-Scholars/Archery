

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABasePawn/ABasePawn.h"
#include "ArcheryPawn.generated.h"

#include "ABaseController/ABaseController.h"

UCLASS()
class ARCHERY_API AArcheryPawn : public ABasePawn {
	GENERATED_BODY()
private:
	ABaseController* m_pBowHand = nullptr;

public:
	// Constructor
	AArcheryPawn();
	// PreInit
	virtual void PreInit() override;

	// Set bow hand
	void SetBowHand(ABaseController* controller);

	// Get arrow hand
	ABaseController* GetArrowHand();

	// Trigger pressed
	virtual void ABaseController::OnR_TRIGGER_Pressed() override;
	//virtual void ABasePawn::OnL_TRIGGER_Pressed() override;
};
