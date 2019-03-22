

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

			// if target is about to break
			if (target->m_bWillBreak) {
				// just delete and respawn
				target->m_bDeletable = true;
			}

			FVector loc = target->GetActorLocation();	
			FRotator rot = target->GetActorRotation();

			target->m_Move.NextMovement(&loc, &rot);
			target->SetActorLocation(loc);

			// rotate targets accordingly
			if (target->m_Move.IsRotating()) target->SetActorRotation(rot);
		}

		// destroy any necessary targets
		if (target->m_bDeletable) {
			AArcheryTarget* x = m_aTargets[i];
			m_aTargets.Remove(x);
			x->m_Move.FreezeMovement();
			x->DestroyEntity();
		}

	}

	if (m_bSpawning) {
		
		if (m_aTargets.Num() < m_iMaxTargets) {

			// random spawn location
			FVector startPoint = randBoundLoc();
			FVector endPoint = FVector(0, 0, 0);

			AArcheryTarget* target = (AArcheryTarget*)GetWorld()->SpawnActor(AArcheryTarget::StaticClass(), &startPoint);

			if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.Medium) {
				
				// movement

				do {
					endPoint = randBoundLoc();
				} while ( (endPoint - startPoint).Size() < TARGET_VICINITY_THRESHOLD);

				target->m_Move.SetSpeed(g_archeryGlobals.m_iDifficulty);
				target->m_Move.Move(startPoint, endPoint);

				// rotation

				if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.ExtremelyHard) {
					target->m_Move.Rotate(target->GetActorRotation());
				}

			}

			m_aTargets.Add(target);

		}

	}
}
