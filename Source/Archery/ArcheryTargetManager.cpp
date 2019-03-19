

#include "ArcheryTargetManager.h"
#include "Archery.h"


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
	
	// delete each target
	for (AArcheryTarget* target : m_aTargets) {
		target->Deactivate(100, 5);
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

	for (int i = 0; i < m_aTargets.Num(); i++) {
		AArcheryTarget* target = m_aTargets[i];

		// move targets accordingly
		if (target->m_Move.IsMoving()) {
			FVector loc = target->GetActorLocation();
			target->m_Move.NextMovement(&loc);
			target->SetActorLocation(loc);
		}

		// destroy any necessary targets
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

			// if difficulty is high enough...
			if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.Medium) {
				target->m_Move.Move(target->GetActorLocation(), randBoundLoc());
			}

			m_aTargets.Add(target);

		}

	}
}
