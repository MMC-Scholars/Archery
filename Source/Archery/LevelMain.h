

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/TextRenderComponent.h"
#include "ArcheryTargetManager.h"
#include "LevelMain.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API ALevelMain : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	// Constructor
	ALevelMain();

	virtual void PostInit() override;
	virtual void DefaultThink() override;

	// Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score Text")
		UTextRenderComponent* m_pScore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target Manager")
		AArcheryTargetManager* m_pTargetManager;

	// Functions

	void SetScore(int score);

	void StartGame();

};
