

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/BoxComponent.h"
#include "ArcheryTarget.h"
#include "ArcheryTargetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AArcheryTargetManager : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	// Constructor
	AArcheryTargetManager();

	// Components
	UBoxComponent *m_pSpawnVol;

	bool m_bSpawning;

	TArray<AArcheryTarget*> m_aTargets;

	int m_iMaxTargets;

	// Functions

	void BeginSpawn(int numTargets);
	void EndSpawn();

	FVector randBoundLoc();

	virtual void DefaultThink() override;

};
