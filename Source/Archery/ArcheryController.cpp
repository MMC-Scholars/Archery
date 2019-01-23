
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

	// only if holding the bow in the other hand

	if (g_archeryGlobals.getBowHand()) {
		/*
		// detatch all actors that are not the bow
		if (this == g_archeryGlobals.getBowHand()) {
			if (m_iButtonsPressed & IN_TRIGGER) {
				for (int i = 0; i < m_aOverlapActors.Num(); i++) {
					ABow* bow = Cast<ABow>(m_aOverlapActors[i]);
					if (!bow) m_aOverlapActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				}
			}
		}
		*/

		if (this == g_archeryGlobals.getArrowHand()) {

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
	}

	// Super

	Super::OnButtonsChanged();

}

// Set ABasePawn's controller class
static ControllerClassGetter g_overrideControllerClassGetter(AArcheryController::StaticClass(), 1);