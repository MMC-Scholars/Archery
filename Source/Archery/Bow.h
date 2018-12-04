

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Bow.generated.h"

UCLASS()
class ARCHERY_API ABow : public APickup {
	GENERATED_BODY()

public:
	// PreInit
	virtual void PreInit() override;

	// Pickup
	virtual void OnPickup_Implementation(ABaseController* controller) override;
	// Drop
	virtual void OnDrop_Implementation(ABaseController* controller) override;
};
