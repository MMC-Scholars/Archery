

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Archery.h"

#define MAX_NUM_TARGETS 20
#define MAX_NUM_ARROWS 15

ALevelMain::ALevelMain() {
	g_archeryGlobals.m_iScore = 0;

	m_pScore = CreateDefaultSubobject<UTextRenderComponent>("Score Tracker");
	m_pScore->SetTextRenderColor(FColor::White);
	m_pScore->SetXScale(1.0f);
	m_pScore->SetYScale(1.0f);
	m_pScore->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
}

void ALevelMain::PostInit() {
	StartGame();
}

void ALevelMain::DefaultThink() {
	SetScore(g_archeryGlobals.m_iScore);
}

void ALevelMain::SetScore(int score) {

	char buffer[50];

	snprintf(buffer, sizeof(buffer), "%d", score);

	char str[100];
	
	strcpy_s(str, "Score: ");
	strcat_s(str, buffer);
	strcat_s(str, "\nTime: ");
	strcat_s(str, buffer);

	m_pScore->SetText(FText::AsCultureInvariant(str));
}

void ALevelMain::StartGame() {
	m_pTargetManager->BeginSpawn(MAX_NUM_TARGETS);
}