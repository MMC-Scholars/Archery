

#include "Arrow.h"

//#define OCULUS_TOUCH_CONTROLLER_MODEL_RIGHT_PATH L"StaticMesh'/Game/Meshes/Controllers/oculus_cv1_controller_right.oculus_cv1_controller_right'"
#define ARROW_MESH L"StaticMesh'/Game/Meshes/Arrow.Arrow'"

AArrow::AArrow() {
	m_pPickupMeshComponent->SetStaticMesh(FindMesh(ARROW_MESH));
	//m_pRightControllerMesh = FindMesh(OCULUS_TOUCH_CONTROLLER_MODEL_RIGHT_PATH);

}

void AArrow::PreInit() {
	// Super
	APickup::PreInit();

}


