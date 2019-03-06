
#include "ArcheryController.h"
#include "ABasePawn/ABasePawn.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Bow.h"

void AArcheryController::PreInit() {
	m_iArrowIndex = 0;
}

void AArcheryController::OnButtonsChanged() {

	// if this is the arrow hand and the trigger is pressed
	if (g_archeryGlobals.getBowHand() && m_iButtonsPressed & IN_TRIGGER) {
		FVector loc = GetActorLocation();
				
		// if the current number of arrows is less than the max number of arrows
		if (g_archeryGlobals.m_aArrows.Num() < MAX_NUM_ARROWS) {
			// spawn arrow
			AArrow* currentArrow = (AArrow*)GetWorld()->SpawnActor(AArrow::StaticClass(), &loc);
			g_archeryGlobals.m_aArrows.Add(currentArrow);
		}
		// the max number of arrows has been reached 
		else {
			// reset arrow at index
			g_archeryGlobals.m_aArrows[m_iArrowIndex]->ResetArrow(loc);
			// shift arrow index
			m_iArrowIndex = (m_iArrowIndex >= MAX_NUM_ARROWS - 1) ? 0 : m_iArrowIndex + 1;
		}

	}

	// Super. This is placed after the overridden functionality so that
	// we can spawn an arrow before we attach it to the controller.
	Super::OnButtonsChanged();

}