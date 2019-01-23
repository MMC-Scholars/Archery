

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"
#include <cmath>

#include "System/NLogger.h"

// bowstring attachments relative to the bow mesh
const FVector BOW_TOP = FVector(-8, 0, 53);
const FVector BOW_BOT = FVector(-8, 0, -53);
const FVector BOW_MID = (BOW_TOP+BOW_BOT)/2;

ABow::ABow() {
	// setup bowstring attachment components

	m_pStringTop = CreateDefaultSubobject<USceneComponent>("String Top");
	m_pStringTop->SetRelativeLocation(BOW_TOP);
	m_pStringTop->SetupAttachment(RootComponent);

	m_pStringMid = CreateDefaultSubobject<USceneComponent>("String Middle");
	m_pStringMid->SetRelativeLocation(BOW_MID);
	m_pStringMid->SetupAttachment(RootComponent);

	m_pStringBot = CreateDefaultSubobject<USceneComponent>("String Bottom");
	m_pStringBot->SetRelativeLocation(BOW_BOT);
	m_pStringBot->SetupAttachment(RootComponent);

}

void ABow::PreInit() {

	// reset hands before the game starts
	g_archeryGlobals.resetHands();

	// reset attached arrow
	m_pNotchedArrow = nullptr;
	m_fArrowVelocity = 0;

	// setup bowstring properties
	m_sStringProps.Color = FColor::FromHex("CCC");
	m_sStringProps.Thickness = 0.2f;
	m_sStringProps.Duration = 0.1f;
}

void ABow::OnPickup_Implementation(ABaseController* controller) {

	AActor* hand = (AActor*)controller; // manual C++ cast because Unreal's cast macro freaks out otherwise

	// override default pickup
	if (m_aParentActors.Num() > 1) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_aParentActors.Remove(controller);
		AttachToActor(m_aParentActors[0], FAttachmentTransformRules::KeepWorldTransform);
		hand = (AActor*)m_aParentActors[0];
	}
	
	// override default attachment to align bow properly

	if (hand) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		// bow hand and arrow hand

		AArcheryController* bowHand = Cast<AArcheryController>(hand);
		if (bowHand) {
			g_archeryGlobals.setHands(bowHand);
		}
	}

}

void ABow::OnDrop_Implementation(ABaseController* controller) {

	// clear bow and arrow hand assignments
	if (m_aParentActors.Num() == 0) {
		g_archeryGlobals.resetHands();
	}
}

void ABow::ArrowNotch(AArrow* arrow) {
	m_pNotchedArrow = arrow;
	m_fArrowVelocity = 0;
}

void ABow::DefaultThink() {

	// TODO - move bowstring draw to appropriate place(s)
	
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
				// draw string
				UTIL_DrawLine(m_pStringTop->GetComponentLocation(), s, &m_sStringProps);
				UTIL_DrawLine(m_pStringBot->GetComponentLocation(), s, &m_sStringProps);

												 
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
				float frq = FMath::Clamp(m_fArrowVelocity/1000 + (m_fArrowVelocity-prevArrowVelocity)/1000, 0.0f, 1.0f);
				float amp = FMath::Clamp(FMath::Abs((m_fArrowVelocity - prevArrowVelocity) / 5), 0.0f, 1.0f);
				
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp, g_archeryGlobals.getArrowHand()->m_eWhichHand);

			}
			else { // arrow has just been fired
				
				// TODO - string spring
				
				m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(false);

				//for (int i = 0; i < 5; i++) { Msg("ARROW HAS JUST BEEN FIRED."); }
				
				m_pNotchedArrow->FireArrow(m_fArrowVelocity, forward);
				
				m_pNotchedArrow = nullptr;

				// reset haptics
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getArrowHand()->m_eWhichHand);

			}
			
		}
		else { // there is no notched arrow
			// draw string
			UTIL_DrawLine(m_pStringTop->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
			UTIL_DrawLine(m_pStringBot->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
		}
	}
	else { // if bow is dropped
		// draw string
		UTIL_DrawLine(m_pStringTop->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
		UTIL_DrawLine(m_pStringBot->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);

		g_archeryGlobals.resetHands();
		
		TArray<AActor*> attached;
		GetAttachedActors(attached);
		for (int i = 0; i < attached.Num(); i++) {
			attached.Pop()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		
	}
}
