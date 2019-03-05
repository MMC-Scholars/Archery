
#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseController/ABaseController.h"
#include "Arrow.h"
#include "ArcheryController.generated.h"

UCLASS()
class ARCHERY_API AArcheryController : public ABaseController {
	GENERATED_BODY()
	
public:

	// keeps track of which arrow is being used
	int m_iArrowIndex = 0;

	// override
	virtual void OnButtonsChanged() override;
};
