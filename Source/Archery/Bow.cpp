

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"
#include <cmath>
#include "System/EHandle.h"
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

	// why is it so complicated to get the relative location of a component?
	m_vInitStringLoc = GetTransform().InverseTransformPosition(m_pStringMid->GetComponentLocation());

	m_pStringBot = CreateDefaultSubobject<USceneComponent>("String Bottom");
	m_pStringBot->SetRelativeLocation(BOW_BOT);
	m_pStringBot->SetupAttachment(RootComponent);

	m_pPickupMeshComponent->SetSimulatePhysics(false);

	if (!RootComponent) SetRootComponent(m_pPickupMeshComponent);
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

	ResetBow();

	m_pPickupMeshComponent->SetRenderCustomDepth(false);
}

void ABow::ResetBow() {
	// reset bowstring location
	m_pStringMid->SetRelativeLocation(m_vInitStringLoc);
	if (HasValidRootComponent()) DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	else SetRootComponent(m_pPickupMeshComponent);
	//g_pLeftController->m_aAttachActors.Remove(this);
	//g_pRightController->m_aAttachActors.Remove(this);
	// clear haptics
	GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_pRightController->m_eWhichHand);
	GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_pLeftController->m_eWhichHand);
	m_bHapticPulse = false;
	// clear bow and arrow hands
	m_aParentActors.Empty();
	g_archeryGlobals.resetHands();
	// reset arrow
	if (m_pNotchedArrow != nullptr) {
		if (m_pNotchedArrow->HasValidRootComponent()) m_pNotchedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		else m_pNotchedArrow->SetRootComponent(m_pNotchedArrow->m_pPickupMeshComponent);
		m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(true);
	}
	m_pNotchedArrow = nullptr;
	// disable physics
	m_pPickupMeshComponent->SetSimulatePhysics(false);
	m_pPickupMeshComponent->SetRenderCustomDepth(true);
}

void ABow::Pickup(ABaseController* controller) {
	// clear help string to hide help text
	if (m_pHelpText) m_pHelpText->GetTextRender()->SetText(FText::FromString(ANSI_TO_TCHAR("")));
	if (m_pHelpText2) m_pHelpText2->GetTextRender()->SetText(FText::FromString(ANSI_TO_TCHAR("")));

	m_pPickupMeshComponent->SetSimulatePhysics(false);

	// if no hands are holding a bow
	if (!g_archeryGlobals.getBowHand()) {
		// if controller is only holding the bow
		if (controller->m_aAttachActors.Num() == 1) {
			AttachToActor(controller, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			m_aParentActors.Add(controller);

			// set bow and arrow hands
			AArcheryController* bowHand = Cast<AArcheryController>(controller);
			if (bowHand) g_archeryGlobals.setHands(bowHand, this);
		}
		else controller->m_aAttachActors.Remove(this);
	}

	// a hand is already holding a bow,
	// remove bow from controller's list of attached actors
	else controller->m_aAttachActors.Remove(this);

}

void ABow::OnDrop_Implementation(ABaseController* controller) {
	ResetBow();
}

void ABow::ArrowNotch(AArrow* arrow) {
	// notch arrow
	arrow->m_bIsNotched = true;
	// set notched arrow
	m_pNotchedArrow = arrow;
}

void ABow::DefaultThink() {
	// render bowstring
	UTIL_DrawLine(m_pStringTop->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
	UTIL_DrawLine(m_pStringBot->GetComponentLocation(), m_pStringMid->GetComponentLocation(), &m_sStringProps);
	
	// if arrow hand is set and arrow is notched
	if (g_archeryGlobals.getArrowHand() && m_pNotchedArrow) {

		// get bow hand location
		FVector bLoc = g_archeryGlobals.getBowHand()->GetActorLocation();
		// get arrow hand location
		FVector aLoc = g_archeryGlobals.getArrowHand()->GetActorLocation();

		// get arrow forward and rotation vectors
		FVector forward = FVector(bLoc.X - aLoc.X, bLoc.Y - aLoc.Y, bLoc.Z - aLoc.Z).GetSafeNormal(1);
		forward.Normalize(1);
		FRotator aRot = forward.Rotation();

		// if arrow is notched
		if (m_pNotchedArrow->m_bIsNotched) {

			// max distance from bow
			FVector vMaxNotch = bLoc + (-1 * forward * HALF_ARROW_LENGTH * 2);

			if (((aLoc.X > vMaxNotch.X && aLoc.X > bLoc.X) || (aLoc.X < vMaxNotch.X && aLoc.X < bLoc.X)) &&
				((aLoc.Y > vMaxNotch.Y && aLoc.Y > bLoc.Y) || (aLoc.Y < vMaxNotch.Y && aLoc.Y < bLoc.Y)) &&
				((aLoc.Z > vMaxNotch.Z && aLoc.Z > bLoc.Z) || (aLoc.Z < vMaxNotch.Z && aLoc.Z < bLoc.Z))) {
				aLoc = vMaxNotch;
			}

			// set arrow location and rotation
			m_pNotchedArrow->SetActorRotation(aRot);
			m_pNotchedArrow->SetActorLocation(aLoc + (forward * HALF_ARROW_LENGTH));

			// set bowstring location
			m_pStringMid->SetWorldLocation(aLoc);

			// store previous velocty for haptics
			float prevArrowVelocity = m_fArrowVelocity;

			// calculate velocity
			// d = sqrt(dx^2 + dy^2 + dz^2)
			float distance = sqrt(pow(bLoc.X - aLoc.X, 2) + pow(bLoc.Y - aLoc.Y, 2) + pow(bLoc.Z - aLoc.Z, 2));
			m_fArrowVelocity = 0.1 * pow(distance, 2);

			// haptics
			float frq = FMath::Clamp(m_fArrowVelocity / 1000 + (m_fArrowVelocity - prevArrowVelocity) / 1000, 0.01f, 0.4f);
			float amp = FMath::Clamp(FMath::Abs((m_fArrowVelocity - prevArrowVelocity) / 5), 0.1f, 1.0f);
			// increased arrow hand haptics
			GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp, g_archeryGlobals.getArrowHand()->m_eWhichHand);
			// minimized bow hand haptics
			GetWorld()->GetFirstPlayerController()->SetHapticsByValue(frq, amp / 2, g_archeryGlobals.getBowHand()->m_eWhichHand);
		
			// prepare for haptic bow pulse
			m_fHapticPulseTime = g_pGlobals->curtime;
		
			// arrow post process
			m_pNotchedArrow->m_pPickupMeshComponent->SetRenderCustomDepth(true);

		}

		// arrow is no longer notched
		// and will be fired
		else {
			// disable arrow post process
			m_pNotchedArrow->m_pPickupMeshComponent->SetRenderCustomDepth(false);

			// fire arrow
			m_pNotchedArrow->FireArrow(m_fArrowVelocity, forward);
			m_pNotchedArrow = nullptr;

			// reset bowstring location
			m_pStringMid->SetRelativeLocation(m_vInitStringLoc);

			// turn off bow hand haptics
			GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getBowHand()->m_eWhichHand);

			// enable pulse for arrow hand haptics
			m_bHapticPulse = true;
		}

	}

	// haptic pulse after the arrow has just been shot
	if (m_bHapticPulse) {
		const float PULSE_TIME = 0.06f;
		// set arrow hand haptics
		GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0.3, 1, g_archeryGlobals.getArrowHand()->m_eWhichHand);

		if ((g_pGlobals->curtime - m_fHapticPulseTime) > PULSE_TIME) {
			// reset haptics
			GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getArrowHand()->m_eWhichHand);
			m_bHapticPulse = false;
		}
	}

}
