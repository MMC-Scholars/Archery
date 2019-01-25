

#include "LevelMain.h"
#include "System/NLogger.h"

ALevelMain::ALevelMain() {
	m_iScore = 0;

	m_pScore = CreateDefaultSubobject<UTextRenderComponent>("Score Tracker");
	//m_pScore->SetText("Score: %d", m_iScore);
	m_pScore->SetTextRenderColor(FColor::White);
	m_pScore->SetXScale(1.0f);
	m_pScore->SetYScale(1.0f);
	m_pScore->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
}

void ALevelMain::PostInit() {


}

void ALevelMain::DefaultThink() {

}

