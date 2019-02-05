

#include "ArcheryTarget.h"
#include "System/NLogger.h"
#include "Archery.h"

#define DESTRUCTIBLE_MESH L"DestructibleMesh'/Game/Meshes/target_DM.target_DM'"
const float DEACTIVATE_TIME_SEC = 0.5;

AArcheryTarget::AArcheryTarget() {
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh>destructMesh(DESTRUCTIBLE_MESH);

	m_pTargetMesh = CreateDefaultSubobject<UDestructibleComponent>("Target");
	m_pTargetMesh->SetDestructibleMesh(destructMesh.Object);

	RootComponent = m_pTargetMesh;

	m_pTargetMesh->bGenerateOverlapEvents = true;
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

void AArcheryTarget::Deactivate(float force) {
	m_bActive = false;
	m_bMoving = false;

	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force/10, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);

	m_fDeactivateTime = g_pGlobals->curtime;

	m_bDeactivation = true;
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
			if ((g_pGlobals->curtime - m_fDeactivateTime) > DEACTIVATE_TIME_SEC) {
				m_bDeactivation = false;
				m_pTargetMesh->SetSimulatePhysics(false);
			}
		}
	}
}