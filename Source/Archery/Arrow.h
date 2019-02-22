

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Arrow.generated.h"

UCLASS()
class ARCHERY_API AArrow : public APickup {
	GENERATED_BODY()
	
public:
	// Constructor
	AArrow();
	
	// PreInit
	virtual void PreInit() override;

	// DefaultThink
	virtual void DefaultThink() override;

	// Components
	UBoxComponent* m_pHeadCollision;
	USphereComponent* m_pTailCollision;
	UParticleSystemComponent* m_pParticleSystem;

	UAudioComponent* m_pArrowSoundComponent;
	USoundCue* m_pCueArrowShoot;

	// Variables
	bool m_bTipOverlap;
	bool m_bIsNotched;
	bool m_bIsFired;

	float m_fVelocity;
	FVector m_vForward;

	// Reset
	void ResetArrow(FVector loc);

	// Pickup attachment
	void OnPickup_Implementation(ABaseController* controller) override;
	// Drop fire
	void OnDrop_Implementation(ABaseController* controller) override;

	// Fire arrrow
	void FireArrow(float velocity, FVector forward);

	// Tail overlap
	UFUNCTION()
		void OnOverlapBeginHead(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapBeginTail(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
};

extern const int HALF_ARROW_LENGTH;