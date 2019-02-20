
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

// the maximum number of targets activated at once
#define MAX_NUM_TARGETS 20

// the countdown time (seconds) before the game starts
#define COUNTDOWN_TIME 11

// the amount of time (seconds) to slow down stop the game.
#define ENDING_SLOW_SOUND_TIME 2

// the time (seconds) of one game
#define INITIAL_TIME 20
	//TODO change this to 60

// the max number of high scores displayed
#define NUM_HIGH_SCORES 20

// the path to the score binary file
#define SCORE_FILE "../../scores.scores"