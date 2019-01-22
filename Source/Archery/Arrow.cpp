

#include "Arrow.h"
#include "System/NLogger.h"
#include "ArcheryController.h"
#include "Archery.h"
#include "ArrowPhysics.h"
#include "Bow.h"

#define ARROW_MESH L"StaticMesh'/Game/Meshes/Arrow.Arrow'"
const int HALF_ARROW_LENGTH = 38;

AArrow::AArrow() {
	m_pPickupMeshComponent->SetStaticMesh(FindMesh(ARROW_MESH));
	
	// Arrow Head Collision Sphere
	
	m_pHeadCollision = CreateDefaultSubobject<USphereComponent>("Head Collision");
	m_pHeadCollision->InitSphereRadius(0.5f);

	m_pHeadCollision->SetupAttachment(m_pPickupMeshComponent);
	m_pHeadCollision->SetRelativeLocation(FVector(HALF_ARROW_LENGTH, 0, 0));

	m_pHeadCollision->bGenerateOverlapEvents = true;
	m_pHeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBeginHead);
	
	
	// Arrow Tail Collision Sphere

	m_pTailCollision = CreateDefaultSubobject<USphereComponent>("Tail Collision");
	m_pTailCollision->InitSphereRadius(9.0f);

	m_pTailCollision->SetupAttachment(m_pPickupMeshComponent);
	m_pTailCollision->SetRelativeLocation(FVector(-1 * (HALF_ARROW_LENGTH - 5), 0, 0));

	m_pTailCollision->bGenerateOverlapEvents = true;
	m_pTailCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBeginTail);

	// States
	m_bTipOverlap = false;
	m_bIsNotched = false;
	m_bIsFired = false;

	m_pHeadCollision->SetVisibility(true, true); // debug only
	m_pHeadCollision->SetHiddenInGame(false, true); // debug only
	m_pTailCollision->SetVisibility(true, true); // debug only
	m_pTailCollision->SetHiddenInGame(false, true); // debug only
}

void AArrow::PreInit() {
	// Super
	APickup::PreInit();

}

void AArrow::OnPickup_Implementation(ABaseController* controller) {
	if (m_bTipOverlap) m_bTipOverlap = false;
	if (m_bIsNotched) m_bIsNotched = false;
	if (m_bIsFired) m_bIsFired = false;
	

	// if user is holding bow, use custom pickup attachment
	AActor* hand = (AActor*) controller; // manual C++ cast because Unreal's cast macro freaks out otherwise
	if (hand && g_archeryGlobals.getBowHand()) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		// TODO
		// prevent controller from grabbing multiple arrows at once
		AArcheryController* archeryHand = Cast<AArcheryController>(hand);
		//archeryHand->m_aAttachActors.Remove(this);

		Msg("The arrow hand has %i attached actors. This number should be 0.", m_aAttachActors.Num());

		if (archeryHand && archeryHand->m_aAttachActors.Num() == 0) {
			m_pPickupMeshComponent->SetRenderCustomDepth(false);
			m_pPickupMeshComponent->SetSimulatePhysics(false);
			AttachToActor(hand, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			// manually move arrow mesh forward to appear as though the user is holding the arrow tail 
			m_pPickupMeshComponent->SetRelativeLocation(FVector(HALF_ARROW_LENGTH, 0, 0));
		//	archeryHand->m_aAttachActors.Add(this);
		}
		else {
			m_pPickupMeshComponent->SetRenderCustomDepth(true);
			m_pPickupMeshComponent->SetSimulatePhysics(true);
		}
	}
}

void AArrow::OnOverlapBeginTail(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// TODO change this to overlap with bow string
	ABow* bow = Cast<ABow>(OtherActor);
	if (bow && !m_bIsNotched && !m_bIsFired) {
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToActor(bow, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		bow->ArrowNotch(this);
		m_bIsNotched = true;
	}

}

void AArrow::OnDrop_Implementation(ABaseController* controller) {
	if (m_bIsNotched) {
		m_bIsNotched = false;

		m_pPickupMeshComponent->SetSimulatePhysics(false);
	}
}

void AArrow::FireArrow(float velocity, FVector forward) {
	m_fVelocity = velocity;
	m_vForward = forward;
	m_bIsFired = true;

	
}


void AArrow::OnOverlapBeginHead(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (m_bIsFired) m_bTipOverlap = true;
}

void AArrow::DefaultThink() {
	
	if (m_bIsFired) {
		FVector pos = GetActorLocation();
		FRotator rot = GetActorRotation();

		CalcNextMove(m_fVelocity, m_vForward, pos, rot);


		SetActorLocation(pos);
		SetActorRotation(rot);

		if (m_bTipOverlap) m_bIsFired = false;
	}
	
}