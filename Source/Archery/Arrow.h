

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Arrow.generated.h"

UCLASS()
class ARCHERY_API AArrow : public APickup {
	GENERATED_BODY()
	
public:
	// Constructor
	AArrow();
	// PreInit
	virtual void PreInit() override;

	
	
	
};
