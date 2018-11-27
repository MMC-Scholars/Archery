

#include "Bow.h"
#include "ArcheryPawn.h"
#include "Arrow.h"
#include "APickup/APickup.h"

ABow::ABow() {}

void ABow::PreInit() {}

void ABow::PostInit() {}

void ABow::OnPickup_Implementation(ABaseController* controller) {
	
	// override default attachment to align bow properly
	
	AActor* hand = (AActor*) controller; // manual C++ cast because Unreal's cast macro freaks out otherwise
	if (hand) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	// bow hand and arrow hand

	AArcheryPawn* player = Cast<AArcheryPawn>(g_pBasePawn);
	player->SetBowHand(controller);

	ABaseController* arrowHand = player->GetArrowHand();

	// spawn arrow

	FVector loc = FVector(0, 0, 50);
	AArrow* firstArrow = (AArrow*) GetWorld()->SpawnActor(AArrow::StaticClass(), &loc);

	firstArrow->m_pPickupMeshComponent->SetSimulatePhysics(false);
	firstArrow->AttachToActor(arrowHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//firstArrow->m_pPickupMeshComponent->SetRelativeLocation(FVector(0, 0, 20));
	firstArrow->SetActorRelativeLocation(FVector(40, 0, 0));

	Msg("grabbed");


}

void ABow::OnDrop_Implementation(ABaseController* controller) {
	AArcheryPawn* player = Cast<AArcheryPawn>(g_pBasePawn);

	// remove all attached arrows

	ABaseController* arrowHand = player->GetArrowHand();
	TArray<AActor*> attachedActors;
	arrowHand->GetAttachedActors(attachedActors);

	for (int i = 0; i < attachedActors.Num(); i++) {
		AArrow* arrow = Cast<AArrow>(attachedActors[i]);
		if (arrow) {
			arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			arrow->m_pPickupMeshComponent->SetSimulatePhysics(true);
		}
	}


	// clear bow hand

	player->SetBowHand(nullptr);


	Msg("dropped");
}
