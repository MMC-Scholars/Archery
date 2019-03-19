#pragma once

#include "UnrealEngine.h"

class MovingComponent {

private:
	FVector m_vStart;
	FVector m_vEnd;

	FVector m_vForward;

	bool m_bIsMoving;

	int m_iSpeedFactor;

public:
	// Constructor
	MovingComponent();


	// functions
	void Move(FVector start, FVector end);
	bool IsMoving();

	void SetSpeed(int speed);

private:

	bool PointIsOnLine(FVector pos, FVector start, FVector end);

	FVector ClosestPoint(FVector pos, FVector start, FVector end);

public:

	void NextMovement(FVector* currentPosition);

};