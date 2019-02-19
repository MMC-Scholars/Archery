
#pragma once

#include "CoreMinimal.h"
#include "ArcheryController.h"
#include "System/NLogger.h"

class ArcheryGlobals {

private:
	AArcheryController* m_BowHand;
	AArcheryController* m_ArrowHand;

public:

	// Variables

	unsigned int m_iScore;

	// Functions

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

#define MAX_NUM_TARGETS 20
#define COUNTDOWN_TIME 11
#define ENDING_SLOW_SOUND_TIME 2
#define INITIAL_TIME 60