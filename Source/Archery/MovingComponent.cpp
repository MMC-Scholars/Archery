
#include "MovingComponent.h"
#include "System/NLogger.h"

MovingComponent::MovingComponent() {
	m_bIsMoving = false;
	m_iSpeedFactor = 2;
}

void MovingComponent::Move(FVector start, FVector end) {
	m_vStart = start;
	m_vEnd = end;

	m_fScaleFactor = 0.001;
	m_fInterp = 0;

	m_bIsMoving = true;
}

bool MovingComponent::IsMoving() { return m_bIsMoving; }

void MovingComponent::SetSpeed(int speed) { m_iSpeedFactor = speed; }

FVector MovingComponent::Interpolate(FVector start, FVector end, int(*easing)(int x)) {

	// default linear easing function
	if (!easing) easing = [](int x) { return x; };
	
	FVector delta = end - start;

	// change scale factor accordingly
	if (m_fInterp > 1 || m_fInterp < 0) m_fScaleFactor *= -1;

	// update interp value
	m_fInterp += m_fScaleFactor * m_iSpeedFactor;

	return start + delta * m_fInterp;
}


void MovingComponent::NextMovement(FVector* pos) {
	// only if moving, update position
	if (m_bIsMoving) *pos = Interpolate(m_vStart, m_vEnd);
}







/*
void (MovingComponent::* pMemberFunc)(bool) = NULL;

void playground() {
	MovingComponent mc;
	MovingComponent* pMc = &mc;
	(pMc->*pMemberFunc)(false);

	int j = 0;

	auto pFunc = [](int i) { Msg("%i", i); };


}

void SayHello(FString name) {
	Msg(L"Hello %s", WCStr(name));
}

void SayGoodbye(FString name, bool newline) {
	Msg(L"Goodbye %s", WCStr(name));
	if (newline) Msg("\n");
}

void(*g_pSaying)(FString) = &SayHello;

void playground(int) {
	(*g_pSaying)("Bob");
	g_pSaying = &SayGoodbye;
	g_pSaying("Bob");
}
*/