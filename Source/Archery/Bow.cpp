

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"

#include "System/NLogger.h"

// for test
//#include "StaticMesh.h"

void ABow::PreInit() {

	// reset hands before the game starts
	g_archeryGlobals.resetHands();

	// reset attached arrow
	m_pNotchedArrow = nullptr;

}

void ABow::OnPickup_Implementation(ABaseController* controller) {

	// override default attachment to align bow properly
	
	AActor* hand = (AActor*) controller; // manual C++ cast because Unreal's cast macro freaks out otherwise
	if (hand) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	// bow hand and arrow hand

	AArcheryController* bowHand = Cast<AArcheryController>(controller);
	if (bowHand) {
		g_archeryGlobals.setHands(bowHand);
	}
}

void ABow::OnDrop_Implementation(ABaseController* controller) {
	
	// clear bow and arrow hand assignments

	g_archeryGlobals.resetHands();
}

void ABow::ArrowNotch(AArrow* arrow) {
	m_pNotchedArrow = arrow;
}

void ABow::DefaultThink() {
	if (g_archeryGlobals.getBowHand()) {
		if (m_pNotchedArrow) {
			// get the bow hand origin
			FVector e = g_archeryGlobals.getBowHand()->GetActorLocation();
			// get arrow hand origin
			FVector s = g_archeryGlobals.getArrowHand()->GetActorLocation();

			// forward unit vector
			FVector forward = FVector(e.X - s.X, e.Y - s.Y, e.Z - s.Z);
			forward.GetSafeNormal(1.0f);
			forward.Normalize(1.0f);

			//Msg(forward);

			if (m_pNotchedArrow->m_bIsNotched) { // arrow is notched				
				// set arrow location and rotation
				FRotator rot = forward.Rotation();
				m_pNotchedArrow->SetActorRotation(rot);

				FVector loc = s + (forward * HALF_ARROW_LENGTH);
				m_pNotchedArrow->SetActorLocation(loc);

			}
			else { // arrow has just been fired

				// TODO - dummy value
				float v = 1.0;

				m_pNotchedArrow->FireArrow(v, forward);
				
				m_pNotchedArrow = nullptr;
			}
		}
		else { // there is no notched arrow
		}
	}
	else { // if bow is dropped
		g_archeryGlobals.resetHands();
		
		TArray<AActor*> attached;
		GetAttachedActors(attached);
		for (int i = 0; i < attached.Num(); i++) {
			attached.Pop()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		
	}
}
