

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

	// disable destructible collision
	m_pTargetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pTargetMesh->bGenerateOverlapEvents = false;

	// calculate collision bounds based on static mesh
	m_pCollision = CreateDefaultSubobject<UBoxComponent>("Collision Box");
	
	FVector bounds = m_pTargetMesh->DestructibleMesh->SourceStaticMesh->GetBounds().GetBox().GetSize() / 2;
	m_pCollision->SetBoxExtent(bounds); // rescaling

	m_pCollision->OnComponentBeginOverlap.AddDynamic(this, &AArcheryTarget::OnTargetOverlap);
	m_pTargetMesh->OnComponentFracture.AddUniqueDynamic(this, &AArcheryTarget::OnFracture);

	m_pCollision->SetupAttachment(RootComponent);

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
	m_bDeactivation = false;
	m_bBreakByPlayer = false;

	m_Move = MovingComponent();

	m_pCollision->bGenerateOverlapEvents = true;
	m_pTargetMesh->SetSimulatePhysics(false);

	m_fDeactivateTime = 0;

}

void AArcheryTarget::Deactivate(float force, float time) {
	// turn off overlap
	m_pCollision->bGenerateOverlapEvents = false;

	// play sound
	if (m_bBreakByPlayer) m_pHitCueComponent->Play();
	m_bBreakByPlayer = false;

	m_bActive = false;
	// break target
	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);
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

void AArcheryTarget::OnFracture(const FVector& HitPoint, const FVector& HitDirection) {
	// deactivate target if accidentally broken (bug fix)
	Deactivate(0);
}

void AArcheryTarget::DefaultThink() {
	// Active
	if (m_bActive) {
		if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.Medium) {
			//TODO move targets here using bMoving
		}

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