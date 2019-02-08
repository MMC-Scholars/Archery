

#pragma once

#include "CoreMinimal.h"
#include "VRBase/AWorldButton/AWorldButton.h"
#include "LevelMain.h"
#include "StartButton.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AStartButton : public AWorldButton
{
	GENERATED_BODY()

public:

	// on pressed
	virtual void OnPressed_Implementation(ABaseController* pController) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Main")
	ALevelMain* m_pLevelMain;

};
