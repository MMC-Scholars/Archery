

#include "Arrow.h"
#include "System/NLogger.h"
#include "ABasePawn/ABasePawn.h"
#include "ArcheryController.h"
#include "Archery.h"
#include "ArrowPhysics.h"
#include "Bow.h"
#include "ArcheryTarget.h"
#include "ArcheryTargetManager.h"
#include "Kismet/GameplayStatics.h"

#define ARROW_MESH L"StaticMesh'/Game/Meshes/Arrow.Arrow'"

#define PARTICLE_SYSTEM L"ParticleSystem'/Game/Meshes/materials/p_arrow.p_arrow'"
#define PARTICLE_MAT L"Material'/Game/Meshes/materials/m_arrow_particle.m_arrow_particle'"

#define CUE_ARROW_SHOOT L"SoundCue'/Game/sounds/C_ArrowShoot.C_ArrowShoot'"

const int HALF_ARROW_LENGTH = 37;

AArrow::AArrow() {
	m_pPickupMeshComponent->SetStaticMesh(FindMesh(ARROW_MESH));

	// null RootComponent bug fix
	this->SetRootComponent(m_pPickupMeshComponent);
	
	// Arrow Head Collision Box
	
	m_pHeadCollision = CreateDefaultSubobject<UBoxComponent>("Head Collision");
	
	m_pHeadCollision->SetupAttachment(RootComponent);
	m_pHeadCollision->SetRelativeLocation(FVector(HALF_ARROW_LENGTH-10, 0, 0));
	m_pHeadCollision->SetBoxExtent(FVector(10, 1, 1));
	
	m_pHeadCollision->bGenerateOverlapEvents = true;
	m_pHeadCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBeginHead);
	
	// Arrow Tail Collision Sphere

	m_pTailCollision = CreateDefaultSubobject<USphereComponent>("Tail Collision");
	m_pTailCollision->InitSphereRadius(9.0f);

	m_pTailCollision->SetupAttachment(RootComponent);
	m_pTailCollision->SetRelativeLocation(FVector(-1 * (HALF_ARROW_LENGTH - 5), 0, 0));

	m_pTailCollision->bGenerateOverlapEvents = true;
	m_pTailCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBeginTail);

	// Particle System

	m_pParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("Particle System");
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(PARTICLE_SYSTEM);
	m_pParticleSystem->SetTemplate(PS.Object);
	m_pParticleSystem->SetRelativeLocation(FVector(-1 * (HALF_ARROW_LENGTH - 5), 0, 0));
	
	m_pParticleSystem->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> Material(PARTICLE_MAT);
	m_pParticleSystem->SetMaterial(0, (UMaterialInterface*) Material.Object);

	// shoot
	static ConstructorHelpers::FObjectFinder<USoundCue> CueArrowShoot(CUE_ARROW_SHOOT);
	m_pCueArrowShoot = CueArrowShoot.Object;

	// null RootComponent bug fix
	if (GetParentActor() != NULL) {
		GetParentActor()->SetRootComponent(m_pPickupMeshComponent);
	}
}

void AArrow::PreInit() {
	// reset arrow
	ResetArrow(GetActorLocation());
}

void AArrow::ResetArrow(FVector loc) {
	// detach from all actors
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// simulate physics
	m_pPickupMeshComponent->SetRenderCustomDepth(true);
	m_pPickupMeshComponent->SetSimulatePhysics(true);
	// reset particle system
	m_pParticleSystem->Deactivate();
	// set actor location
	SetActorLocation(loc);

	// reset states
	m_bIsNotched = false;
	m_bIsFired = false;
}

void AArrow::Pickup(ABaseController* controller) {
	m_pPickupMeshComponent->SetSimulatePhysics(false);
	
	// add controller to list of parent actors
	m_aParentActors.Add(controller);
	
	// if the bow is being held
	if (g_archeryGlobals.getBowHand()) {
		// if this is not the bow hand
		if (controller != g_archeryGlobals.getBowHand()) {
			// if no arrows are held
			if (!g_archeryGlobals.getBow()->m_pNotchedArrow) {
				AttachToActor(controller, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				// manually move mesh forward to appear as though the user is holding the arrow tail 
				m_pPickupMeshComponent->SetRelativeLocation(FVector(HALF_ARROW_LENGTH, 0, 0));
			}
			// if this is not the arrow is already held
			else if (this != g_archeryGlobals.getBow()->m_pNotchedArrow)
				m_pPickupMeshComponent->SetSimulatePhysics(true);

		}
	}
	// if the bow is not being held
	else AttachToActor(controller, FAttachmentTransformRules::KeepWorldTransform);
}

void AArrow::OnDrop_Implementation(ABaseController* controller) {
	// if the arrow is notched
	if (m_bIsNotched) {
		m_pPickupMeshComponent->SetSimulatePhysics(false);

		m_bIsNotched = false;

	}
}

void AArrow::OnOverlapBeginTail(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		// if overlapping with the bow
		if (OtherActor->IsA(ABow::StaticClass())) {
			// if arrow is held in arrow hand 
			if (m_aParentActors.Contains(g_archeryGlobals.getArrowHand())) {
				// notch arrow
				Cast<ABow>(OtherActor)->ArrowNotch(this);
			}
		}
	}
}

void AArrow::FireArrow(float velocity, FVector forward) {
	// set fired arrow variables
	m_fVelocity = velocity;
	m_vForward = forward;
	
	// detach from parent
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// enable arrow particles
	m_pParticleSystem->Activate();

	// play arrow shoot sound
	UGameplayStatics::PlaySoundAtLocation(this, m_pCueArrowShoot, GetActorLocation());

	// arrow is now fired
	m_bIsFired = true;
}

void AArrow::DefaultThink() {

	if (m_bIsFired) {

		FVector pos = GetActorLocation();
		FRotator rot = GetActorRotation();

		// calculate movement based on arrow physics
		CalcNextMove(m_fVelocity, m_vForward, pos, rot);

		SetActorLocation(pos);
		SetActorRotation(rot);

	}

}

void AArrow::OnOverlapBeginHead(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (m_bIsFired && !OtherActor->IsA(AArrow::StaticClass())) {

			// if arrow hits target
			if (OtherActor->IsA(AArcheryTarget::StaticClass())) {

			}
			// if arrow hits TargetManager, ignore
			else if (OtherActor->IsA(AArcheryTargetManager::StaticClass())) {}
			// if arrow hits bow, ignore
			else if (OtherActor->IsA(ABow::StaticClass())) {}
			// if arrow hits base pawn, ignore
			else if (OtherActor->IsA(ABasePawn::StaticClass())) {}
			// if arrow hits controllers, ignore
			else if (OtherActor->IsA(AArcheryController::StaticClass())) {}
			// if arrow hits anything else, stop
			else {
				m_pParticleSystem->Deactivate();
				m_bIsFired = false;
			}

		}
	}
}
