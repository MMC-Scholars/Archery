

#include "ArcheryTarget.h"
#include "System/NLogger.h"
#include "Archery.h"
#include "Arrow.h"

#define DESTRUCTIBLE_MESH L"DestructibleMesh'/Game/Meshes/target_DM.target_DM'"
#define HIT_CUE L"SoundCue'/Game/sounds/C_TargetHit.C_TargetHit'"

AArcheryTarget::AArcheryTarget() {
	m_bDeletable = false;
	m_fDeactivateTimeFinal = DEACTIVATE_TIME_SEC;

	// destructible mesh
	m_pTargetMesh = CreateDefaultSubobject<UDestructibleComponent>("Target");
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh>destructMesh(DESTRUCTIBLE_MESH);
	m_pTargetMesh->SetDestructibleMesh(destructMesh.Object);

	m_pTargetMesh->CastShadow = false;

	RootComponent = m_pTargetMesh;
	
	// collision
	m_pTargetMesh->bGenerateOverlapEvents = true;
	m_pTargetMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_pTargetMesh->OnComponentBeginOverlap.AddDynamic(this, &AArcheryTarget::OnTargetOverlap);

	// hit sounds
	static ConstructorHelpers::FObjectFinder<USoundCue> HitCue(HIT_CUE);
	m_pHitCueComponent = CreateDefaultSubobject<UAudioComponent>("Hit Cue Component");
	m_pHitCueComponent->SetSound(HitCue.Object);

	m_pHitCueComponent->bAutoActivate = false;
}

void AArcheryTarget::PreInit() {
	Activate();
}

void AArcheryTarget::Activate() {
	m_bActive = true;
	m_bMoving = false;
	m_bDeactivation = false;
	m_bBreakByPlayer = false;

	m_pTargetMesh->SetSimulatePhysics(false);

	m_fDeactivateTime = 0;

}

void AArcheryTarget::Deactivate(float force, float time) {
	// turn off overlap
	m_pTargetMesh->bGenerateOverlapEvents = false;

	// play sound
	if (m_bBreakByPlayer) m_pHitCueComponent->Play();
	m_bBreakByPlayer = false;

	m_bActive = false;
	m_bMoving = false;
	// break target
	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force/10, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);
	// timer
	m_fDeactivateTimeFinal = time;
	m_fDeactivateTime = g_pGlobals->curtime;

	m_bDeactivation = true;
}

void AArcheryTarget::OnTargetOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		
		// if arrow hits target
		if (OtherActor->GetClass() == AArrow::StaticClass()) {
			AArrow* arrow = Cast<AArrow>(OtherActor);
			if (arrow) {
				m_bBreakByPlayer = true;
				// deactivate target
				Deactivate(arrow->m_fVelocity);
				// increment score
				g_archeryGlobals.m_iScore++;

			}
		}

	}
}

void AArcheryTarget::DefaultThink() {
	// Active
	if (m_bActive) {

		//TODO move targets here using bMoving
	}
	// Inactive
	else {
		// Deactivation
		if (m_bDeactivation) {

			if ((g_pGlobals->curtime - m_fDeactivateTime) > m_fDeactivateTimeFinal) {
				m_bDeactivation = false;
				m_pTargetMesh->SetSimulatePhysics(false);
				m_bDeletable = true;
			}

		}
	}
}