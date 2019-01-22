
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Bow.h"
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
		
			// if controller is not near a pickup, or is near the bow, spawn an arrow

			bool nearBow = false;
			for (int i = 0; i < m_aOverlapActors.Num(); i++) {
				ABow* bow = Cast<ABow>(m_aOverlapActors[i]);
				if (bow) nearBow = true;
			}

			if (m_aOverlapActors.Num() == 0 || nearBow) {
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