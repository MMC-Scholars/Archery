

#include "ArcheryTarget.h"
#include "System/NLogger.h"
#include "Archery.h"

#define DESTRUCTIBLE_MESH L"DestructibleMesh'/Game/Meshes/target_DM.target_DM'"

AArcheryTarget::AArcheryTarget() {
	m_bDeletable = false;
	m_fDeactivateTimeFinal = DEACTIVATE_TIME_SEC;

	static ConstructorHelpers::FObjectFinder<UDestructibleMesh>destructMesh(DESTRUCTIBLE_MESH);

	m_pTargetMesh = CreateDefaultSubobject<UDestructibleComponent>("Target");
	m_pTargetMesh->SetDestructibleMesh(destructMesh.Object);

	RootComponent = m_pTargetMesh;
	
	m_pTargetMesh->bGenerateOverlapEvents = true;
	m_pTargetMesh->OnComponentBeginOverlap.AddDynamic(this, &AArcheryTarget::OnTargetOverlap);
	
}

void AArcheryTarget::PreInit() {
	Activate();
}

void AArcheryTarget::Activate() {
	m_bActive = true;
	m_bMoving = false;
	m_bDeactivation = false;

	m_pTargetMesh->SetSimulatePhysics(false);

	m_fDeactivateTime = 0;

	
}

void AArcheryTarget::Deactivate(float force, float time) {
	m_bActive = false;
	m_bMoving = false;

	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force/10, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);

	m_fDeactivateTimeFinal = time;
	m_fDeactivateTime = g_pGlobals->curtime;

	m_bDeactivation = true;
}

void AArcheryTarget::OnTargetOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		Msg("\nOVERLAP");
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