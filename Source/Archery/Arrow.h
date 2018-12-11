

#pragma once

#include "CoreMinimal.h"
#include "VRBase/APickup/APickup.h"
#include "Components/SphereComponent.h"
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
	USphereComponent* m_pTailCollision;

	// Variables
	bool m_bIsNotched;
	bool m_bIsFired;

	// Pickup attachment
	void OnPickup_Implementation(ABaseController* controller) override;

	// Tail overlap
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
