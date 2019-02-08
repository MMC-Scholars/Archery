

#include "ArcheryTargetManager.h"


AArcheryTargetManager::AArcheryTargetManager() {
	m_pSpawnVol = CreateDefaultSubobject<UBoxComponent>("Spawn Volume");

	m_bSpawning = false;
}

void AArcheryTargetManager::BeginSpawn(int numTargets) {

	if (!numTargets || numTargets < 1) numTargets = 20;
	m_iMaxTargets = numTargets;

	// start spawning
	m_bSpawning = true;
}

void AArcheryTargetManager::EndSpawn() {
	m_bSpawning = false;
	
	// delete all targets
	for (AArcheryTarget* target : m_aTargets) {
		target->m_bDeletable = true;
	}

}

FVector AArcheryTargetManager::randBoundLoc() {

	FVector bounds = m_pSpawnVol->GetScaledBoxExtent();
	FVector loc = m_pSpawnVol->GetComponentLocation();

	FVector i = loc - (bounds);
	FVector f = loc + (bounds);

	float nx = FMath::RandRange(i.X, f.X);
	float ny = FMath::RandRange(i.Y, f.Y);
	float nz = FMath::RandRange(i.Z, f.Z);

	return FVector(nx, ny, nz);
}

void AArcheryTargetManager::DefaultThink() {

	// destroy any necessary targets
	for (int i = 0; i < m_aTargets.Num(); i++) {
		if (m_aTargets[i]->m_bDeletable) {
			AArcheryTarget* x = m_aTargets[i];
			m_aTargets.Remove(x);
			x->DestroyEntity();
		}
	}

	if (m_bSpawning) {
		
		if (m_aTargets.Num() < m_iMaxTargets) {

			// random spawn location
			FVector loc = randBoundLoc();

			AArcheryTarget* target = (AArcheryTarget*)GetWorld()->SpawnActor(AArcheryTarget::StaticClass(), &loc);

			m_aTargets.Add(target);

		}

	}
}
