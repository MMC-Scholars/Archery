#pragma once

#include "System/NLogger.h"

const float TIME_INTERVAL = 0.001;

// Given a velocity and forward vector, calculates the next incremental position 
// and rotation of the object in the form of a FVector and FRotator.
// updates all parameters involved.
void CalcNextMove(float velocity, FVector forward, FVector pos, FRotator rot) {
	FVector components = velocity*forward;
	
	Msg("forward^");
	Msg(forward);
	Msg("components^");
	Msg(components);
	Msg("\n\n");
	
	//float deg = GetHorDeg(forward);
	
	// get new position for 0.0001 seconds or something

	// then calc forward vector and new vel for those
}
/*
// Given a forward vector, calculates the angle (in degrees)
// from horizontal at which the object is being projected.
// This is calculated regardless of XY direction.
float GetHorDeg(FVector forward) {

}

FVector GetXYPos(FVector forward) {

}
*/