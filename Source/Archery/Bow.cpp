

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"
#include <cmath>

#include "System/NLogger.h"

#define BOW_MESH L"StaticMesh'/Game/Meshes/Bow.Bow'"

// bowstring attachments relative to the bow mesh
const FVector BOW_TOP = FVector(-8, 0, 53);
const FVector BOW_BOT = FVector(-8, 0, -53);
const FVector BOW_MID = (BOW_TOP+BOW_BOT)/2;

ABow::ABow() {

	m_pPickupMeshComponent->SetStaticMesh(FindMesh(BOW_MESH));

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

	m_pPickupMeshComponent->SetSimulatePhysics(false);

}

void ABow::PreInit() {

	// reset hands before the game starts
	g_archeryGlobals.resetHands();

	// reset attached arrow
	m_pNotchedArrow = nullptr;
	m_fArrowVelocity = 0;

	// setup bowstring properties
	m_sStringProps.Color = FColor::FromHex("CCC");
	m_sStringProps.Thickness = 0.4f;
	m_sStringProps.Duration = 0.1f;
}

void ABow::OnPickup_Implementation(ABaseController* controller) {

	// manual C++ cast because Unreal's cast macro freaks out otherwise
	AActor* hand = (AActor*)controller;
	
	// we don't need to very that hand or g_archeryGlobals.getBowHand() 
	//exist because they must exist at the times they are referenced

	// override default pickup
	if (m_aParentActors.Num() > 1) {
		m_aParentActors.Remove(controller);

		// override default attachment to align bow properly
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		
		AttachToActor(g_archeryGlobals.getBowHand(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	} else {

		// override default attachment to align bow properly
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		// bow hand and arrow hand

		AArcheryController* bowHand = Cast<AArcheryController>(hand);
		if (bowHand) g_archeryGlobals.setHands(bowHand);

	}

}

void ABow::OnDrop_Implementation(ABaseController* controller) {
	// clear bow and arrow hand assignments
	if (m_aParentActors.Num() == 0) g_archeryGlobals.resetHands();
}

void ABow::ArrowNotch(AArrow* arrow) {
	// if an arrow is somehow already notched, detatch and simulate physics
	if (m_pNotchedArrow) {
		m_pNotchedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_pNotchedArrow->m_bIsNotched = false;
		m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(true);
	}

	arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	arrow->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	m_pNotchedArrow = arrow;
	m_pNotchedArrow->m_bIsNotched = true;
	m_fArrowVelocity = 0;
	m_bHapticPulse = false;
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

				// verify physics is not simulating (bug fix)
				m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(false);

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
				float frq = FMath::Clamp(m_fArrowVelocity/1000 + (m_fArrowVelocity-prevArrowVelocity)/1000, 0.01f, 0.4f);
				float amp = FMath::Clamp(FMath::Abs((m_fArrowVelocity - prevArrowVelocity) / 5), 0.0f, 1.0f);
				
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp, g_archeryGlobals.getArrowHand()->m_eWhichHand);
				
				// minimized bow hand haptics
				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp/2, g_archeryGlobals.getBowHand()->m_eWhichHand);

				// haptic time
				m_fHapticPulseTime = g_pGlobals->curtime;
			}
			else { // arrow has just been fired

				m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(false);
				m_pNotchedArrow->FireArrow(m_fArrowVelocity, forward);
				m_pNotchedArrow = nullptr;

				GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getBowHand()->m_eWhichHand);

				m_bHapticPulse = true;
			}
			
		}
		else { // there is no notched arrow
			// render string
			UTIL_DrawLine(m_pStringTop->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
			UTIL_DrawLine(m_pStringBot->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
		}
	}
	else { // if bow is dropped
		// render string
		UTIL_DrawLine(m_pStringTop->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
		UTIL_DrawLine(m_pStringBot->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);

		g_archeryGlobals.resetHands();
	
		if (m_pNotchedArrow) m_pNotchedArrow->ResetArrow(m_pNotchedArrow->GetActorLocation());
	
	}

	if (m_bHapticPulse) {
		const float PULSE_TIME = 0.06f;

		GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0.4, 1, g_archeryGlobals.getArrowHand()->m_eWhichHand);
		
		if ( (g_pGlobals->curtime - m_fHapticPulseTime) > PULSE_TIME ) {
			// reset haptics
			GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getArrowHand()->m_eWhichHand);
			
			m_bHapticPulse = false;
		}
	}
}
