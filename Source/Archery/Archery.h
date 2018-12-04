
#pragma once

#include "CoreMinimal.h"
#include "ArcheryController.h"
#include "System/NLogger.h"

class ArcheryGlobals {

private:
	AArcheryController* m_BowHand;
	AArcheryController* m_ArrowHand;

public:
	AArcheryController* getBowHand() {
		return m_BowHand;
	}

	AArcheryController* getArrowHand() {
		return m_ArrowHand;
	}

	void setHands(AArcheryController* bowHand) {
		m_BowHand = bowHand;

		AArcheryController* leftController = static_cast<AArcheryController*>(g_pLeftController);
		AArcheryController* rightController = static_cast<AArcheryController*>(g_pRightController);

		if (m_BowHand == leftController) {
			m_ArrowHand = rightController;
		}
		else {
			m_ArrowHand = leftController;
		}

	}

	void resetHands() {
		m_BowHand = nullptr;
		m_ArrowHand = nullptr;
	}
};

extern ArcheryGlobals g_archeryGlobals;