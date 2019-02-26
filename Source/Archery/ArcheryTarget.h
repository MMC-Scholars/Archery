

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/DestructibleComponent.h"
#include "Engine/DestructibleMesh.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
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

	UBoxComponent* m_pCollision;

	USoundCue* m_pHitCue;
	UAudioComponent* m_pHitCueComponent;

	// Variables
	bool m_bActive;
	bool m_bDeactivation;
	bool m_bMoving;
	bool m_bDeletable; // signifies when the ArcheryTargetManager is able to destroy the actor
	bool m_bBreakByPlayer;

	float m_fDeactivateTime;
	float m_fDeactivateTimeFinal;

	// Functions
	void PreInit() override;

	void Activate();
	#define DEACTIVATE_TIME_SEC 0.5
	void Deactivate(float force, float time = DEACTIVATE_TIME_SEC);
	
	UFUNCTION()
	void OnTargetOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DefaultThink() override;
};

