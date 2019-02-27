
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Bow.h"

void AArcheryController::OnButtonsChanged() {

	Msg("press ma buttons");
	// if holding the bow in the other hand

	if (g_archeryGlobals.getBowHand()) {
		Msg("press ms bowhand");
		// if this is the arrow hand

		if (this == g_archeryGlobals.getArrowHand()) {
			Msg("press my arrow hand");
			// if the trigger was pressed

			if (m_iButtonsPressed & IN_TRIGGER) {
				Msg("mi buttons PRESSED");
				// if controller is not near a pickup, or is near "helping actors", spawn an arrow
				
				bool nearHelpingActor = false;
				for (int i = 0; i < m_aOverlapActors.Num(); i++) {
					Msg("all overlapped actors");
					if (m_aOverlapActors[i]->GetClass() == ABow::StaticClass()) nearHelpingActor = true;
					if (m_aOverlapActors[i]->GetClass() == AArcheryController::StaticClass()) nearHelpingActor = true;
				}

				if (m_aOverlapActors.Num() == 0 || nearHelpingActor) {
					Msg("near helper or somtin");
					FVector loc = this->GetActorLocation();

					// this will never be greater than MAX_NUM_ARROWS but
					// it is good to include this check for consistency.
					if (g_archeryGlobals.m_aArrows.Num() >= MAX_NUM_ARROWS) {
						Msg("max perry arrows");
						// reset index if necessary
						if (m_iArrowIndex > g_archeryGlobals.m_aArrows.Num() - 1) m_iArrowIndex = 0;
						
						g_archeryGlobals.m_aArrows[m_iArrowIndex]->ResetArrow(loc);
					}
					// if m_aArrows.Num() < MAX_NUM_ARROWS
					else {
						Msg("no u bonobo");
						AArrow* currentArrow = (AArrow*)GetWorld()->SpawnActor(AArrow::StaticClass(), &loc);
						g_archeryGlobals.m_aArrows.Add(currentArrow);
					}

					// increment index
					m_iArrowIndex++;

				}
				
			}

		}
	}

	// Super
	Msg("spiderman verse");
	Super::OnButtonsChanged();

}