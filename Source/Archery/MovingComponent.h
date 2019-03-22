#pragma once

#include "UnrealEngine.h"

class MovingComponent {

private:

	FVector m_vStart;
	FVector m_vEnd;

	FRotator m_rStart;

	float m_fScaleFactor;
	float m_fInterp;

	bool m_bIsMoving;
	bool m_bIsRotating;

	float m_fSpeedFactor;

public:

	// Constructor
	MovingComponent();

	// functions

	void FreezeMovement();

	void Move(FVector start, FVector end);
	bool IsMoving();

	void Rotate(FRotator rot);
	bool IsRotating();

	void SetSpeed(float speed);

private:

	FVector Interpolate(FVector start, FVector end, FRotator* rot);

public:

	void NextMovement(FVector* currentPosition, FRotator* currentRotation = &FRotator(0, 0, 0));

};