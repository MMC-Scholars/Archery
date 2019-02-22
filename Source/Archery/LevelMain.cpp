

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Scores.h"
#include "Archery.h"

ALevelMain::ALevelMain() {
	m_pGameMusic = CreateDefaultSubobject<UAudioComponent>("Music");
	m_pGameMusic->bAutoActivate = false;
}

void ALevelMain::PostInit() {
	if (m_pLeaderboardsText) m_pLeaderboardsText->GetTextRender()->SetText(FText::FromString(""));

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

	// read leaderboards, write to TextRender
	char str[256];
	FString highScoreStr = FString("");

	m_aHighScores = ArcheryScores::readScores(NUM_HIGH_SCORES);
	for (int i = m_aHighScores.Num()-1; i >= 0; i--) {
		sprintf_s(str, "%d points\n", m_aHighScores[i]);
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
			float rawTime = COUNTDOWN_TIME - (g_pGlobals->curtime - m_fStartCount);
			m_iDisplayCount = (int) rawTime;

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

				int score = g_archeryGlobals.m_iScore;
				
				// results
				if (m_pResultsText) {
					//FString old = m_pResultsText->GetTextRender()->Text.ToString();
					char str[100];
					sprintf_s(str, "\nYou earned %u points in %d seconds!", score, m_iMaxTime);
					//old.Append(str);
					FString result = FString(str);

					//m_pResultsText->GetTextRender()->SetText(FText::FromString(old));
					m_pResultsText->GetTextRender()->SetText(FText::FromString(result));
				}

				//TODO if score made it into leaderboards, append string saying new record
				m_aHighScores.Sort();
				if (m_aHighScores.Num() < NUM_HIGH_SCORES) m_aHighScores.Add(score);
				else if (score > m_aHighScores[0]) m_aHighScores[0] = score;
				m_aHighScores.Sort();

				ArcheryScores::writeScores(m_aHighScores);

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