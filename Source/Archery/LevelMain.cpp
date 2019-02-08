

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Archery.h"

#define MAX_NUM_TARGETS 20
#define MAX_NUM_ARROWS 15
#define INITIAL_TIME 20
//TODO change this to 60

ALevelMain::ALevelMain() {
	m_pScore = CreateDefaultSubobject<UTextRenderComponent>("Score Tracker");
	m_pScore->SetTextRenderColor(FColor::White);
	m_pScore->SetXScale(1.0f);
	m_pScore->SetYScale(1.0f);
	m_pScore->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
}

void ALevelMain::PostInit() {
	if (m_pTargetManager) ResetGame();
}

void ALevelMain::ResetGame() {
	// targets
	m_pTargetManager->EndSpawn();
	// timer
	m_bIsTiming = false;
	m_fDisplayTime = 0.00;
	// score
	g_archeryGlobals.m_iScore = 0;
	// scoreboard
	SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);
}

void ALevelMain::StartGame() {
	// start spawning targets
	m_pTargetManager->BeginSpawn(MAX_NUM_TARGETS);
	// start timer
	m_fMaxTime = INITIAL_TIME;
	m_fStartTime = g_pGlobals->curtime;
	m_bIsTiming = true;
}

void ALevelMain::SetScoreboard(int score, float time) {

	char scoreBf[sizeof(int)];
	snprintf(scoreBf, sizeof(int), "%d", score);

	char timerBf[sizeof(float)];
	/*
	int time1 = (int) time;
	int time2 = (int) (time * 100 - time1 * 100);
	snprintf(timerBf, sizeof(float), "%2d.%2d", time1, time2);
	*/
	snprintf(timerBf, sizeof(float), "%4.2f", time);

	char str[100];
	
	strcpy_s(str, "Score: ");
	strcat_s(str, scoreBf);
	strcat_s(str, "\nTime: ");
	strcat_s(str, timerBf);

	m_pScore->SetText(FText::AsCultureInvariant(str));
}

void ALevelMain::DefaultThink() {
	if (m_bIsTiming) {
		// timer
		m_fDisplayTime = m_fMaxTime - (g_pGlobals->curtime - m_fStartTime);
		// scoreboard
		SetScoreboard(g_archeryGlobals.m_iScore, m_fDisplayTime);

		// if time is up
		if (m_fDisplayTime <= 0) ResetGame();

	}

}