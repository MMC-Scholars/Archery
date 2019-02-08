

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
	bool m_bDeactivation;
	bool m_bMoving;
	bool m_bDeletable; // signifies when the ArcheryTargetManager is able to destroy the actor

	float m_fDeactivateTime;
	
	// Functions
	void PreInit() override;

	void Activate();
	void Deactivate(float force);

	void DefaultThink() override;
};

