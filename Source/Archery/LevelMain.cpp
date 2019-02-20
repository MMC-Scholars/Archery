

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Scores.h"
#include "Archery.h"

ALevelMain::ALevelMain() {
	m_pGameMusic = CreateDefaultSubobject<UAudioComponent>("Music");
	m_pGameMusic->bAutoActivate = false;
}

void ALevelMain::PostInit() {
	if (m_pTargetManager) ResetGame();
	if (m_pResultsText) (m_pResultsText->GetTextRender())->SetText(FText::FromString(ANSI_TO_TCHAR(" ")));
}

void ALevelMain::ResetGame() {
	// stop music
	m_fEndingStartTime = g_pGlobals->curtime;
	m_bIsEnding = true;
	// targets
	m_pTargetManager->EndSpawn();
	// timer
	m_bIsTiming = false;
	m_bIsCountdown = false;
	m_fDisplayTime = 0.00;
	// score
	g_archeryGlobals.m_iScore = 0;
	// scoreboard
	SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);
	// countdown
	(m_pTimerText->GetTextRender())->SetText( FText::FromString(ANSI_TO_TCHAR(" ")) );

	// leaderboards
	char str[5];
	FString highScoreStr = FString("");
	TArray<int> highScores = ArcheryScores::readScores(NUM_HIGH_SCORES);
	for (int i = 0; i < highScores.Num(); i++) {
		sprintf_s(str, "%d\n", highScores[i]);
		highScoreStr.Append(str);
	}

	m_pLeaderboardsText->GetTextRender()->SetText(FText::FromString(highScoreStr));
	
}

void ALevelMain::StartGame() {
	m_bIsCountdown = true;
	m_fStartCount = g_pGlobals->curtime;
	
	// play music
	if (m_pGameMusicCue) m_pGameMusic->Play();

	// clear results
	(m_pResultsText->GetTextRender())->SetText(FText::FromString(ANSI_TO_TCHAR(" ")));

	m_bIsTiming = true;
}

void ALevelMain::SetScoreboard(int score, float time) {
	char str[100];
	sprintf_s(str, "Score: %d\nTime: %4.2f", score, time);
	(m_pScoreText->GetTextRender())->SetText( FText::FromString(ANSI_TO_TCHAR(str)) );
}

void ALevelMain::DefaultThink() {

	if (m_bIsTiming) {
		if (m_bIsCountdown) {
			// countdown
			m_iDisplayCount = (int) ( COUNTDOWN_TIME - (g_pGlobals->curtime - m_fStartCount) );
			// if time is up
			if (m_iDisplayCount <= 0) {

				(m_pTimerText->GetTextRender())->SetText( FText::FromString(ANSI_TO_TCHAR("GO!")) );
				
				// setup game
				// start spawning targets
				m_pTargetManager->BeginSpawn(MAX_NUM_TARGETS);
				// start timer
				m_iMaxTime = INITIAL_TIME;
				m_fStartTime = g_pGlobals->curtime;

				m_bIsCountdown = false;
			}
			else {
				char str[sizeof(int)];
				sprintf_s(str, "%d", m_iDisplayCount);
				(m_pTimerText->GetTextRender())->SetText( FText::FromString(ANSI_TO_TCHAR(str)) );
			}
		}
		else {
			// timer
			m_fDisplayTime = m_iMaxTime - (g_pGlobals->curtime - m_fStartTime);
			// scoreboard
			SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);

			// if time is up
			if (m_fDisplayTime <= 0) {

				// results
				if (m_pResultsText) {
					//FString old = m_pResultsText->GetTextRender()->Text.ToString();
					char str[100];
					sprintf_s(str, "\nYou earned %u points in %d seconds!", g_archeryGlobals.m_iScore, m_iMaxTime);
					//old.Append(str);
					FString result = FString(str);


					//TODO if score made it into leaderboards, append string saying new record

					//m_pResultsText->GetTextRender()->SetText(FText::FromString(old));
					m_pResultsText->GetTextRender()->SetText(FText::FromString(result));
				}

				// reset game
				ResetGame();
			
			}
		}
	}

	if (m_bIsEnding) {
		float timeElapsed = (g_pGlobals->curtime - m_fEndingStartTime);
		if (timeElapsed >= ENDING_SLOW_SOUND_TIME) {
			if (m_pGameMusicCue) {
				m_pGameMusic->Stop();
				m_pGameMusic->SetPitchMultiplier(1);
			}
			m_bIsEnding = false;
		}
		else {
			float pitch = (ENDING_SLOW_SOUND_TIME - timeElapsed) / ENDING_SLOW_SOUND_TIME;
			m_pGameMusic->SetPitchMultiplier(pitch);
		}

	}

}