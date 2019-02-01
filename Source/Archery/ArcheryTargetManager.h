

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

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Targets)
		int m_iMaxTargets;

	bool m_bSpawning;

	TArray<AArcheryTarget*> m_aTargets;

	// Functions

	void BeginSpawn();

	void EndSpawn();

	virtual void DefaultThink() override;

};
