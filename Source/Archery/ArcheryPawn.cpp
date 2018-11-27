

#include "ArcheryPawn.h"
#include "System/NLogger.h"

#include "ABaseController/ABaseController.h"

#define OCULUS_TOUCH_CONTROLLER_MODEL_LEFT_PATH L"StaticMesh'/Game/Meshes/Controllers/oculus_cv1_controller_left.oculus_cv1_controller_left'"
#define OCULUS_TOUCH_CONTROLLER_MODEL_RIGHT_PATH L"StaticMesh'/Game/Meshes/Controllers/oculus_cv1_controller_right.oculus_cv1_controller_right'"

AArcheryPawn::AArcheryPawn() {
	// Create controller models
	m_pLeftControllerMesh = FindMesh(OCULUS_TOUCH_CONTROLLER_MODEL_LEFT_PATH);
	m_pRightControllerMesh = FindMesh(OCULUS_TOUCH_CONTROLLER_MODEL_RIGHT_PATH);
}

void AArcheryPawn::PreInit() {
	// Super
	ABasePawn::PreInit();
}

void AArcheryPawn::SetBowHand(ABaseController* controller) {
	m_pBowHand = controller;
}

ABaseController* AArcheryPawn::GetArrowHand() {
	ABaseController* arrowHand = nullptr;

	// if a hand is holding a bow
	if (m_pBowHand) {
		if (m_pBowHand == m_pLHand) arrowHand = m_pRHand;
		else if (m_pBowHand == m_pRHand) arrowHand = m_pLHand;
	}

	return arrowHand;
}

//void ABaseController::OnR_TRIGGER_Pressed() {
//}