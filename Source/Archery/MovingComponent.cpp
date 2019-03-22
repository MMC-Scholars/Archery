
#include "MovingComponent.h"
#include "System/NLogger.h"

MovingComponent::MovingComponent() {
	FreezeMovement();
}

void MovingComponent::FreezeMovement() {
	m_fSpeedFactor = 1.0;

	m_bIsMoving = false;
	m_bIsRotating = false;
}

void MovingComponent::Move(FVector start, FVector end) {
	m_vStart = start;
	m_vEnd = end;

	m_fScaleFactor = 0.001;
	m_fInterp = 0.0;

	m_bIsMoving = true;
}

bool MovingComponent::IsMoving() { return m_bIsMoving; }

void MovingComponent::Rotate(FRotator rot) {
	m_rStart = rot;

	m_bIsRotating = true;
}

bool MovingComponent::IsRotating() { return m_bIsRotating; }

void MovingComponent::SetSpeed(float speed) { m_fSpeedFactor = speed; }

FVector MovingComponent::Interpolate(FVector start, FVector end, FRotator* rot) {
	
	FVector delta = end - start;

	// change scale factor accordingly
	if (m_fInterp > 1 || m_fInterp < 0) m_fScaleFactor *= -1;

	// update interp value
	m_fInterp += m_fScaleFactor * m_fSpeedFactor;
	
	// update rotation
	FRotator r = m_rStart;
	if (m_bIsRotating) *rot = FRotator(r.Pitch+1, r.Roll+1, r.Yaw+1) * 360 * m_fInterp;

	return start + delta * m_fInterp;
}

void MovingComponent::NextMovement(FVector* pos, FRotator* rot) {
	// only if moving, update position
	if (m_bIsMoving) *pos = Interpolate(m_vStart, m_vEnd, rot);
}