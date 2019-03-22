
#pragma once

#include "CoreMinimal.h"
#include "ArcheryController.h"
#include "Bow.h"
#include "Arrow.h"
#include "System/NLogger.h"

class ArcheryGlobals {

private:
	AArcheryController* m_BowHand;
	AArcheryController* m_ArrowHand;
	// bow being held
	ABow* m_pBow;

public:

	// Variables

	// difficulty
	enum m_eDifficulty {
		Easy = 0,
		Medium,
		Hard,
		ExtremelyHard,
		Impossible,
		SuperImpossible,
		DMAC,

		NUM_DIFFICULTIES
	};
	int m_iDifficulty;

	// score
	unsigned int m_iScore;
	
	// arrow pool
	TArray<AArrow*> m_aArrows;

	// Functions

	ABow* getBow() {
		return m_pBow;
	}

	AArcheryController* getBowHand() {
		return m_BowHand;
	}

	AArcheryController* getArrowHand() {
		return m_ArrowHand;
	}

	void setHands(AArcheryController* bowHand, ABow* bow) {
		m_pBow = bow;

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
		m_pBow = nullptr;
	}
};

extern ArcheryGlobals g_archeryGlobals;

/*
** GLOBAL PREPROCESSOR DEFINITIONS
*/

// the maximum number of arrows in game at once
#define MAX_NUM_ARROWS 15

// the maximum number of targets activated at once
#define MAX_NUM_TARGETS 15

// the minimum distance between a moving target's start and end point
#define TARGET_VICINITY_THRESHOLD 200

// the countdown time (seconds) before the game starts
#define COUNTDOWN_TIME 11

// the amount of time (seconds) to slow down stop the game.
#define ENDING_SLOW_SOUND_TIME 2

// the initial time (seconds) of one game
#define INITIAL_TIME 60

// the proportion of score to time that must be reached before increasing difficulty
#define SCORE_THRESHOLD 0.7

// the proportion of INITIAL_TIME that will be rewarded to the player on difficulty change
#define TIME_MULT_FACTOR 0.5

// the max number of high scores displayed
#define NUM_HIGH_SCORES 5

// the path to the score binary file
#define SCORE_FILE "../../scores.scores"