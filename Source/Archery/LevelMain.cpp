

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Scores.h"
#include "Archery.h"
#include "Kismet/GameplayStatics.h"

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
	// reset difficulty
	g_archeryGlobals.m_iDifficulty = g_archeryGlobals.Easy;
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

	// reverse read scores to output the biggest scores first
	m_aHighScores = ArcheryScores::readScores();
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

	char *diff;
	switch (g_archeryGlobals.m_iDifficulty) {
		case 6: { diff = "DMac"; break; }
		case 5: { diff = "Super Impossible"; break; }
		case 4: { diff = "Impossible"; break; }
		case 3: { diff = "Extremely Hard"; break; }
		case 2: { diff = "Hard"; break; }
		case 1: { diff = "Medium"; break; }
		default: { diff = "Easy"; break; }
	}

	sprintf_s(str, "Score: %d\nTime: %4.2f\nDifficulty: %s", score, time, diff);
	(m_pScoreText->GetTextRender())->SetText( FText::FromString(ANSI_TO_TCHAR(str)) );
}

void ALevelMain::DefaultThink() {

	if (m_bIsTiming) {

		/* game is counting down . . . */

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
		
		/* game is currently running . . . */
		
		else {
			// timer
			m_fDisplayTime = m_iMaxTime - (g_pGlobals->curtime - m_fStartTime);
			// scoreboard
			SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);
			// restart song if needed
			if (!m_pGameMusic->IsPlaying()) m_pGameMusic->Play();
			
			/* check score for difficulty change */
			
			//if ( (int) g_archeryGlobals.m_iScore >= m_iMaxTime * SCORE_THRESHOLD) {
			if ((int)g_archeryGlobals.m_iScore >= m_iMaxTime * SCORE_THRESHOLD) {
					if (g_archeryGlobals.m_iDifficulty < g_archeryGlobals.NUM_DIFFICULTIES - 1) {
					// increase time
					m_iMaxTime += INITIAL_TIME * TIME_MULT_FACTOR;

					// increase difficulty
					g_archeryGlobals.m_iDifficulty++;

					// increase music pitch and speed
					float pitch = ((float) g_archeryGlobals.m_iDifficulty / 100.0 / 2) + 1.0;
					m_pGameMusic->SetPitchMultiplier(pitch);
				}
			}

			/* if time is up */

			if (m_fDisplayTime <= 0) {

				int score = g_archeryGlobals.m_iScore;
				
				// results
				if (m_pResultsText) {
					char str[100];
					sprintf_s(str, "\nYou earned %u points in %d seconds!", score, m_iMaxTime);
					FString result = FString(str);

					m_pResultsText->GetTextRender()->SetText(FText::FromString(result));
				}

				if (m_aHighScores.Num() < NUM_HIGH_SCORES) m_aHighScores.Add(score);
				else if (score > m_aHighScores[0]) m_aHighScores[0] = score;

				ArcheryScores::writeScores(m_aHighScores);

				// reset game
				ResetGame();
			
			}
		}
	}

	if (m_bIsEnding) {

		/* game is ending . . . */

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