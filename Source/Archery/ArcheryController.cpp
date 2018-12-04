
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"

/*
 * Constructor
 */
AArcheryController::AArcheryController() {

}

void AArcheryController::OnButtonsChanged() {

	m_iButtons |= m_iButtonsPressed;
	m_iButtons &= ~m_iButtonsReleased;

	Msg("%i", m_iButtonsPressed);

	// Super
	Super::OnButtonsChanged();

	if ( this == g_archeryGlobals.getArrowHand() ) {
//		Msg("this is the arrow hand");

		/*
		if (m_iButtonsPressed & IN_TRIGGER) {
			Msg("help! I'm stuck in the computer!");
		}
		*/
	}

}

// Set ABasePawn's controller class
static ControllerClassGetter g_overrideControllerClassGetter(AArcheryController::StaticClass(), 1);