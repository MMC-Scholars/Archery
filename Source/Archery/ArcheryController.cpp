
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Bow.h"

#define MAX_NUM_ARROWS 15

void AArcheryController::OnButtonsChanged() {

	// if holding the bow in the other hand

	if (g_archeryGlobals.getBowHand()) {

		// if this is the arrow hand

		if (this == g_archeryGlobals.getArrowHand()) {

			// if the trigger was pressed

			if (m_iButtonsPressed & IN_TRIGGER) {

				// if controller is not near a pickup, or is near "helping actors", spawn an arrow
				
				bool nearHelpingActor = false;
				for (int i = 0; i < m_aOverlapActors.Num(); i++) {

					if (m_aOverlapActors[i]->GetClass() == ABow::StaticClass()) nearHelpingActor = true;
					if (m_aOverlapActors[i]->GetClass() == AArcheryController::StaticClass()) nearHelpingActor = true;

				}

				if (m_aOverlapActors.Num() == 0 || nearHelpingActor) {

					FVector loc = this->GetActorLocation();

					// this will never be greater than MAX_NUM_ARROWS but
					// it is good to include this check for consistency.
					if (m_aArrows.Num() >= MAX_NUM_ARROWS) {
						// reset index if necessary
						if (m_iArrowIndex > m_aArrows.Num() - 1) m_iArrowIndex = 0;
						
						m_aArrows[m_iArrowIndex]->ResetArrow(loc);
					}
					// if m_aArrows.Num() < MAX_NUM_ARROWS
					else {
						AArrow* currentArrow = (AArrow*)GetWorld()->SpawnActor(AArrow::StaticClass(), &loc);
						m_aArrows.Add(currentArrow);
					}

					// increment index
					m_iArrowIndex++;

				}
				
			}

		}
	}

	// Super

	Super::OnButtonsChanged();

}