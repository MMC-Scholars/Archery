
#pragma once

#include "CoreMinimal.h"
#include "VRBase/AWorldButton/AWorldButton.h"
#include "Bow.h"
#include "ResetBowButton.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AResetBowButton : public AWorldButton
{
	GENERATED_BODY()
	
public:

	// on pressed
	virtual void OnPressed_Implementation(ABaseController* pController) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow Reset")
	ABow* m_pBow;
	
};
