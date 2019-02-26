

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
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
	ATextRenderActor* m_pScoreText;

	// connect to timer TextRender placed in world
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer Text")
	ATextRenderActor* m_pTimerText;

	// connect to ATargetManager placed in world
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target Manager")
	AArcheryTargetManager* m_pTargetManager;

	// connect to timer TextRender placed in world
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leaderboards Text")
	ATextRenderActor* m_pLeaderboardsText;

	// display final score and time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Results Text")
	ATextRenderActor* m_pResultsText;

	// game music
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Button)
	USoundCue* m_pGameMusicCue;
	UAudioComponent* m_pGameMusic;
	
	// difficulty level
	enum Difficulty {
		Easy = 0,
		Medium,
		Hard,
		ExtremelyHard,
		Impossible,
		SuperImpossible,
		DMAC,

		NUM_DIFFICULTIES
	};

	// current game difficulty
	int m_iGameDiff;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

		FName PropertyName = (PropertyChangedEvent.Property != nullptr)
			? PropertyChangedEvent.Property->GetFName() : NAME_None;

		// set sound cue from the editor
		if (m_pGameMusic != nullptr &&
			m_pGameMusicCue != nullptr) {

			m_pGameMusic->SetSound(m_pGameMusicCue);
		}

		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
#endif

	// countdown
	bool m_bIsCountdown;
	float m_fStartCount;
	int m_iDisplayCount;

	// timer
	bool m_bIsTiming;
	float m_fStartTime; // simply stores the start time
	float m_fDisplayTime; // displayed time
	int m_iMaxTime; // stores the maximum amount of time left

	bool m_bIsEnding; // resetting game
	float m_fEndingStartTime;

	// high score array
	TArray<int> m_aHighScores;

	// Functions
	void ResetGame();

	void SetScoreboard(int score, float time);

	void StartGame();

};
