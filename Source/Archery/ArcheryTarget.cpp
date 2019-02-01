

#include "ArcheryTarget.h"
#include "System/NLogger.h"
#include "Archery.h"

#define DESTRUCTIBLE_MESH L"DestructibleMesh'/Game/Meshes/target_DM.target_DM'"

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
	m_pTargetMesh->SetSimulatePhysics(false);

	m_bMoving = false;
}

void AArcheryTarget::Deactivate(float force) {
	m_bActive = false;

	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force/10, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);

	m_bMoving = false;
}

void AArcheryTarget::DefaultThink() {
	if (m_bActive) {

		// Test
		//FVector loc = m_pTargetMesh->GetComponentLocation();
		//m_pTargetMesh->SetRelativeLocation(FVector(loc.X - 0.1, loc.Y + 0.2, loc.Z + 0.05));


	}
}