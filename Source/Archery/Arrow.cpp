

#include "Arrow.h"
#include "System/NLogger.h"
#include "ArcheryController.h"
#include "Archery.h"
#include "Bow.h"

#define ARROW_MESH L"StaticMesh'/Game/Meshes/Arrow.Arrow'"
const int HALF_ARROW_LENGTH = 38;

AArrow::AArrow() {
	m_pPickupMeshComponent->SetStaticMesh(FindMesh(ARROW_MESH));

	// Arrow Tail Collision Sphere

	m_pTailCollision = CreateDefaultSubobject<USphereComponent>("Tail Collision");
	m_pTailCollision->InitSphereRadius(9.0f);

	m_pTailCollision->SetupAttachment(m_pPickupMeshComponent);
	m_pTailCollision->SetRelativeLocation(FVector(-1*(HALF_ARROW_LENGTH-5), 0, 0));

	m_pTailCollision->bGenerateOverlapEvents = true;
	m_pTailCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);

	// States

	m_bIsNotched = false;
	m_bIsFired = false;

	m_pTailCollision->SetVisibility(true, true); // debug only
	m_pTailCollision->SetHiddenInGame(false, true); // debug only
}

void AArrow::PreInit() {
	// Super
	APickup::PreInit();

}

void AArrow::OnPickup_Implementation(ABaseController* controller) {

	

	// if user is holding bow, use custom pickup attachment
	AActor* hand = (AActor*) controller; // manual C++ cast because Unreal's cast macro freaks out otherwise
	if (hand && g_archeryGlobals.getBowHand()) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// prevent controller from grabbing multiple arrows at once
		AArcheryController* archeryHand = Cast<AArcheryController>(controller);
		if (archeryHand->m_aAttachActors.Num() == 0) {
			m_pPickupMeshComponent->SetRenderCustomDepth(false);
			m_pPickupMeshComponent->SetSimulatePhysics(false);
			AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			// manually move arrow mesh forward to appear as though the user is holding the arrow tail 
			m_pPickupMeshComponent->SetRelativeLocation(FVector(HALF_ARROW_LENGTH, 0, 0));
		}
		else {
			m_pPickupMeshComponent->SetRenderCustomDepth(true);
			m_pPickupMeshComponent->SetSimulatePhysics(true);
		}
	}
}

void AArrow::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// TODO change this to overlap with bow string
	ABow* bow = Cast<ABow>(OtherActor);
	if (bow && !m_bIsNotched && !m_bIsFired) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(bow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

}

void AArrow::DefaultThink() {
	Msg("This is the arrow thinking right now");
}
