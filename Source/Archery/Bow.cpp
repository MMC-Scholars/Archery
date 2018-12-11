

#include "Bow.h"
#include "ArcheryController.h"
#include "Arrow.h"
#include "Archery.h"
#include "APickup/APickup.h"

// for test
//#include "StaticMesh.h"

void ABow::PreInit() {

	// reset hands before the game starts
	g_archeryGlobals.resetHands();

}

void ABow::OnPickup_Implementation(ABaseController* controller) {

	// override default attachment to align bow properly
	
	AActor* hand = (AActor*) controller; // manual C++ cast because Unreal's cast macro freaks out otherwise
	if (hand) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	// bow hand and arrow hand

	AArcheryController* bowHand = Cast<AArcheryController>(controller);
	if (bowHand) {
		g_archeryGlobals.setHands(bowHand);
	}
}

void ABow::OnDrop_Implementation(ABaseController* controller) {
	
	// clear bow and arrow hand assignments

	g_archeryGlobals.resetHands();
}
