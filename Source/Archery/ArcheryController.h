
#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseController/ABaseController.h"
#include "ArcheryController.generated.h"

UCLASS()
class ARCHERY_API AArcheryController : public ABaseController {
	GENERATED_BODY()
	
private:

public:
	// Constructor
	AArcheryController();
	
	void OnButtonsChanged() override;
};
