

#include "StartButton.h"
#include "System/NLogger.h"

void AStartButton::OnPressed_Implementation(ABaseController* pController) {

	//TODO - add a delay here?
	if (m_pLevelMain && !m_pLevelMain->m_bIsTiming) m_pLevelMain->StartGame();
	
}


