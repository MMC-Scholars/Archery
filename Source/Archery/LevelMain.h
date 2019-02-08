

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

	// UProperty only to change the material rendering the score
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score Text")
		UTextRenderComponent* m_pScore;

	// connect to ATargetManager placed in world
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target Manager")
		AArcheryTargetManager* m_pTargetManager;

	// timer
	bool m_bIsTiming;
	float m_fStartTime; // simply stores the start time
	float m_fDisplayTime; // displayed time
	int m_fMaxTime; // stores the maximum amount of time left

	// Functions
	void ResetGame();

	void SetScoreboard(int score, float time);

	void StartGame();

};
