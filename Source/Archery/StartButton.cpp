

#include "StartButton.h"
#include "System/NLogger.h"

void AStartButton::OnPressed_Implementation(ABaseController* pController) {

	if (m_pLevelMain && !m_pLevelMain->m_bIsTiming) m_pLevelMain->StartGame();
	
}


