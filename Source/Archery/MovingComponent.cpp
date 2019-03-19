
#include "MovingComponent.h"

MovingComponent::MovingComponent() {
	m_vEnd = FVector(0, 0, 0);

	m_bIsMoving = false;

	m_iSpeedFactor = 1;
}

void MovingComponent::Move(FVector start, FVector end) {
	m_vStart = start;
	m_vEnd = end;

	// calculate forward unit vector
	//todo make simpler
	m_vForward = m_vEnd - m_vStart;
	m_vForward.GetSafeNormal(1);
	m_vForward.Normalize(1);

	m_bIsMoving = true;
}

bool MovingComponent::IsMoving() {
	return m_bIsMoving;
}

void MovingComponent::SetSpeed(int speed) {
	m_iSpeedFactor = speed;
}

bool MovingComponent::PointIsOnLine(FVector pos, FVector start, FVector end) {
	// why Unreal doesn't include a function like this is beyond me.
	return (
		(pos.X >= start.X && pos.X <= end.X
		&& pos.Y >= start.Y && pos.Y <= end.Y
		&& pos.Z >= start.Z && pos.Z <= end.Z)
		||
		(pos.X <= start.X && pos.X >= end.X
		&& pos.Y <= start.Y && pos.Y >= end.Y
		&& pos.Z <= start.Z && pos.Z >= end.Z)
	);
}

FVector MovingComponent::ClosestPoint(FVector pos, FVector start, FVector end) {
	
	FVector point = FVector(0, 0, 0);

	FVector v1 = pos - start;
	FVector v2 = pos - end;

	if (v1.Size() > v2.Size()) point = end;
	else point = start;

	return point;
}

void MovingComponent::NextMovement(FVector* pos) {
	// only if moving
	if (m_bIsMoving) {

		// destination reached
		if (!PointIsOnLine(*pos, m_vStart, m_vEnd)) {
			*pos = ClosestPoint(*pos, m_vStart, m_vEnd);
			m_vForward *= -1;
		}
		
		// continue moving
		else {
			*pos += m_iSpeedFactor * m_vForward;
		}

	}
}