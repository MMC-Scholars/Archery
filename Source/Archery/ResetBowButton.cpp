
#include "ResetBowButton.h"
#include "Archery.h"


void AResetBowButton::OnPressed_Implementation(ABaseController* pController) {
	m_pBow->ResetBow();
	/*
	// detatch and remove bow
	m_pBow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	g_pLeftController->m_aAttachActors.Remove(m_pBow);
	g_pRightController->m_aAttachActors.Remove(m_pBow);

	// clear bow and arrow hand assignments
	m_pBow->m_aParentActors.Empty();
	g_archeryGlobals.resetHands();

	// clear haptics
	GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getBowHand()->m_eWhichHand);
	GetWorld()->GetFirstPlayerController()->SetHapticsByValue(0, 0, g_archeryGlobals.getArrowHand()->m_eWhichHand);

	// reset attached arrow
	if (m_pBow->m_pNotchedArrow) {
		if (m_pBow->m_pNotchedArrow->GetAttachParentActor())
			m_pBow->m_pNotchedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		m_pBow->m_pNotchedArrow->m_pPickupMeshComponent->SetSimulatePhysics(true);
	}

	m_pBow->m_pNotchedArrow = nullptr;
	m_pBow->m_fArrowVelocity = 0;

	// reset location and rotation
	m_pBow->SetActorLocation(m_pBow->m_initLoc);
	m_pBow->SetActorRotation(m_pBow->m_initRot);

	// turn off physics
	m_pBow->m_pPickupMeshComponent->SetSimulatePhysics(false);
	m_pBow->m_pPickupMeshComponent->SetRenderCustomDepth(true);
	*/
}


