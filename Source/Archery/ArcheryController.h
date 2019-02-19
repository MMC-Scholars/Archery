
#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseController/ABaseController.h"
#include "Arrow.h"
#include "ArcheryController.generated.h"

UCLASS()
class ARCHERY_API AArcheryController : public ABaseController {
	GENERATED_BODY()
	
public:

	// arrow pool
	TArray<AArrow*> m_aArrows;
	int m_iArrowIndex = 0;

	virtual void OnButtonsChanged() override;
};
