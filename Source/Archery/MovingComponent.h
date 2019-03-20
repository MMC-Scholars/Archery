#pragma once

#include "UnrealEngine.h"

class MovingComponent {

private:

	FVector m_vStart;
	FVector m_vEnd;

	float m_fScaleFactor;
	float m_fInterp;

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

	FVector Interpolate(FVector start, FVector end, int(*easing)(int x) = nullptr);

public:

	void NextMovement(FVector* currentPosition);

};