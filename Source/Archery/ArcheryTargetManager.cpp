

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

	//int active = 0;
	//int moving = 0;

	for (int i = 0; i < m_aTargets.Num(); i++) {
		AArcheryTarget* target = m_aTargets[i];

		// move targets accordingly
		if (target->m_Move.IsMoving()) {
			//todo check if near other target
			//FVector loc1 = target->m_Move.m_vStart;
			//FVector loc2 = target->m_Move.m_vEnd;
			//Msg("from (%f, %f, %f) to (%f, %f, %f)", loc1.X, loc1.Y, loc1.Z, loc2.X, loc2.Y, loc2.Z);
			
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

		//if (target->m_bActive) active++;
		//if (target->m_Move.IsMoving()) moving++;

	}

	//Msg("Num targets: %d", m_aTargets.Num());
	//Msg("Num targets active: %d", active);
	//Msg("Num targets moving: %d", moving);


	if (m_bSpawning) {
		
		if (m_aTargets.Num() < m_iMaxTargets) {

			// random spawn location
			FVector startPoint = randBoundLoc();
			FVector endPoint = FVector(0, 0, 0);

			AArcheryTarget* target = (AArcheryTarget*)GetWorld()->SpawnActor(AArcheryTarget::StaticClass(), &startPoint);

			// if difficulty is high enough...
			//if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.Medium) {
			if (g_archeryGlobals.m_iDifficulty >= g_archeryGlobals.Easy) {

				//todo set speed				

				do {
					endPoint = randBoundLoc();
				} while ( (endPoint - startPoint).Size() < TARGET_VICINITY_THRESHOLD);

				target->m_Move.Move(startPoint, endPoint);
			}

			m_aTargets.Add(target);

		}

	}
}
