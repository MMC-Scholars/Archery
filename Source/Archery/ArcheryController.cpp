
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Arrow.h"

/*
 * Constructor
 */
AArcheryController::AArcheryController() {

}

void AArcheryController::OnButtonsChanged() {

	if ( this == g_archeryGlobals.getArrowHand() ) {

		// On trigger press, hold arrow

		if (m_iButtonsPressed & IN_TRIGGER) {
		
			// if user is not already near a pickup, spawn an arrow

			if (m_aOverlapActors.Num() == 0) {
				FVector loc = this->GetActorLocation();
				AArrow* currentArrow = (AArrow*)GetWorld()->SpawnActor(AArrow::StaticClass(), &loc);
			}
		
		}

		
	}

	// Super

	Super::OnButtonsChanged();

}

// Set ABasePawn's controller class
static ControllerClassGetter g_overrideControllerClassGetter(AArcheryController::StaticClass(), 1);