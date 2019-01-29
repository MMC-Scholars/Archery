

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Archery.h"

ALevelMain::ALevelMain() {
	g_archeryGlobals.m_iScore = 0;

	m_pScore = CreateDefaultSubobject<UTextRenderComponent>("Score Tracker");
	m_pScore->SetTextRenderColor(FColor::White);
	m_pScore->SetXScale(1.0f);
	m_pScore->SetYScale(1.0f);
	m_pScore->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
}

void ALevelMain::PostInit() {

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