

#include "ArcheryTargetManager.h"


AArcheryTargetManager::AArcheryTargetManager() {
	m_pSpawnVol = CreateDefaultSubobject<UBoxComponent>("Spawn Volume");

	if (!m_iMaxTargets || m_iMaxTargets < 0) m_iMaxTargets = 20;

	m_bSpawning = false;

	//TODO - remove this later
	BeginSpawn();
}

void AArcheryTargetManager::BeginSpawn() {
	m_bSpawning = true;
}

void AArcheryTargetManager::EndSpawn() {
	m_bSpawning = false;
}

void AArcheryTargetManager::DefaultThink() {
	if (m_bSpawning) {

		if (m_aTargets.Num() < m_iMaxTargets) {
			//TODO - random location
			FVector loc = this->GetActorLocation();

			AArcheryTarget* target = (AArcheryTarget*)GetWorld()->SpawnActor(AArcheryTarget::StaticClass(), &loc);

			m_aTargets.Add(target);

		}

	}
}
