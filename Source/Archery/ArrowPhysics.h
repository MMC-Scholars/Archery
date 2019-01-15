#pragma once

#include "System/NLogger.h"
#include <cmath>

// time interval between each calculation
const float TI = 0.14;
// gravity constant
const float G = -9.8;
// Given a velocity vector, forward vector, initial position and initial rotation, 
// calculates the next incremental position and rotation of the object
// in the form of a FVector and FRotator. Updates all parameters involved.
void CalcNextMove(float& velocity, FVector& forward, FVector& ipos, FRotator& irot) {
	FVector vi = velocity*forward;

	// X position
	//
	// Xf = Xi + Vix*t
	float xf = ipos.X + vi.X * TI;


	// Y position
	//
	// Yf = Yi + Viy*t
	float yf = ipos.Y + vi.Y * TI;


	// Z position
	//
	// Zf = Zi + viy*t + 0.5*g*t^2
	float zf = ipos.Z + vi.Z * TI + 0.5 * G * pow(TI, 2);
	

	// Update velocity parameter
	//
	// Vfz = Viz + g*t
	float vfz = vi.Z + G * TI;
	// ||v|| = Sqrt(vfx^2 + vfy^2 + vfz^2)
	velocity = sqrt(pow(vi.X, 2) + pow(vi.Y, 2) + pow(vfz, 2));


	// Update forward vector parameter
	forward = FVector(vi.X, vi.Y, vfz);
	forward.GetSafeNormal(1.0f);
	forward.Normalize(1.0f);


	// Update position parameter
	ipos = FVector(xf, yf, zf);


	// Update rotation parameter
	irot = forward.Rotation();
	

}
