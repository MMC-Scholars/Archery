

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/DestructibleComponent.h"
#include "Engine/DestructibleMesh.h"
#include "ArcheryTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AArcheryTarget : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	// Constructor
	AArcheryTarget();

	// Components
	UDestructibleComponent* m_pTargetMesh;

	// Variables
	bool m_bActive;
	
	// Functions
	void PreInit() override;

	void Activate();
	void Deactivate(float force);

	void DefaultThink() override;
};

