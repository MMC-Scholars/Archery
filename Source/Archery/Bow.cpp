

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"
#include <cmath>

#include "System/NLogger.h"

// for test
//#include "StaticMesh.h"

void ABow::PreInit() {

	// reset hands before the game starts
	g_archeryGlobals.resetHands();

	// reset attached arrow
	m_pNotchedArrow = nullptr;
	m_fArrowVelocity = 0;

	// haptics
	//m_iHapticFac = 0;
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
	m_fArrowVelocity = 0;
	//m_iHapticFac = 90;
}

void ABow::DefaultThink() {

	// if holding bow and arrow
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

			

			if (m_pNotchedArrow->m_bIsNotched) { // arrow is notched				
				// set arrow location and rotation
				FRotator rot = forward.Rotation();
				m_pNotchedArrow->SetActorRotation(rot);

				FVector loc = s + (forward * HALF_ARROW_LENGTH);
				m_pNotchedArrow->SetActorLocation(loc);

				float prevArrowVelocity = m_fArrowVelocity;

				// arrow velocity
				// d = sqrt(dx^2 + dy^2 + dz^2)
				float distance = sqrt(pow(e.X - s.X, 2) + pow(e.Y - s.Y, 2) + pow(e.Z - s.Z, 2));
				m_fArrowVelocity = 0.1 * pow(distance, 2);
				
				// haptics
				/*
				float amplitude = m_fArrowVelocity / 500.0;
				if (amplitude > 1.0) amplitude = 1.0;
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0.01, amplitude, g_archeryGlobals.getArrowHand()->m_eWhichHand);
				*/

				//float frq = FMath::Clamp(m_fArrowVelocity/1000 + (m_fArrowVelocity-prevArrowVelocity)/1000, 0.0f, 1.0f);
				float frq = 1-FMath::Clamp(m_fArrowVelocity / 100 + (m_fArrowVelocity - prevArrowVelocity) / 100, 0.0f, 0.7f);
				float amp = FMath::Clamp(FMath::Abs((m_fArrowVelocity - prevArrowVelocity) / 5), 0.0f, 1.0f);
				
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp, g_archeryGlobals.getArrowHand()->m_eWhichHand);

			}
			else { // arrow has just been fired

				m_pNotchedArrow->FireArrow(m_fArrowVelocity, forward);
				
				m_pNotchedArrow = nullptr;

				// reset haptics
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getArrowHand()->m_eWhichHand);

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
