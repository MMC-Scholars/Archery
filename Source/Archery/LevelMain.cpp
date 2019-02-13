

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Archery.h"

#define MAX_NUM_TARGETS 20
#define COUNTDOWN_TIME 11
#define INITIAL_TIME 60

ALevelMain::ALevelMain() {
	m_pGameMusic = CreateDefaultSubobject<UAudioComponent>("Music");
	m_pGameMusic->bAutoActivate = false;
}

void ALevelMain::PostInit() {
	if (m_pTargetManager) ResetGame();
}

void ALevelMain::ResetGame() {
	// stop music
	if (m_pGameMusicCue) m_pGameMusic->Stop();
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
	(m_pTimerText->GetTextRender())->SetText(" ");
}

void ALevelMain::StartGame() {
	m_bIsCountdown = true;
	m_fStartCount = g_pGlobals->curtime;
	
	// play music
	if (m_pGameMusicCue) m_pGameMusic->Play();

	m_bIsTiming = true;
}

void ALevelMain::SetScoreboard(int score, float time) {
	char str[100];
	sprintf_s(str, "Score: %d\nTime: %4.2f", score, time);
	(m_pScoreText->GetTextRender())->SetText(FText::AsCultureInvariant(str));
}

void ALevelMain::DefaultThink() {

	if (m_bIsTiming) {
		if (m_bIsCountdown) {
			// countdown
			m_iDisplayCount = (int) ( COUNTDOWN_TIME - (g_pGlobals->curtime - m_fStartCount) );
			// if time is up
			if (m_iDisplayCount <= 0) {

				(m_pTimerText->GetTextRender())->SetText("GO!");
				
				// setup game
				// start spawning targets
				m_pTargetManager->BeginSpawn(MAX_NUM_TARGETS);
				// start timer
				m_fMaxTime = INITIAL_TIME;
				m_fStartTime = g_pGlobals->curtime;

				m_bIsCountdown = false;
			}
			else {
				char str[sizeof(int)];
				sprintf_s(str, "%d", m_iDisplayCount);
				(m_pTimerText->GetTextRender())->SetText(str);
			}
		}
		else {
			// timer
			m_fDisplayTime = m_fMaxTime - (g_pGlobals->curtime - m_fStartTime);
			// scoreboard
			SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);

			// if time is up
			if (m_fDisplayTime <= 0) ResetGame();
		}
	}

}