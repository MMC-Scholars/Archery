

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
}

void AArcheryTarget::Deactivate(float force) {
	m_bActive = false;

	m_pTargetMesh->SetSimulatePhysics(true);
	m_pTargetMesh->ApplyDamage(force/10, m_pTargetMesh->GetComponentLocation(), m_pTargetMesh->GetComponentLocation(), force);
}

void AArcheryTarget::DefaultThink() {
	//FVector loc = m_pTargetMesh->GetComponentLocation();
	//m_pTargetMesh->SetRelativeLocation(FVector(loc.X-0.1, loc.Y, loc.Z));

	FRotator rot = m_pTargetMesh->GetComponentRotation();
	m_pTargetMesh->SetRelativeRotation(FRotator(rot.Pitch + 0.1, rot.Yaw + 0.1, rot.Roll + 0.1));

}