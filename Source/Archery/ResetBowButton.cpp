
#include "ResetBowButton.h"
#include "Archery.h"


void AResetBowButton::OnPressed_Implementation(ABaseController* pController) {
	m_pBow->ResetBow();
}


