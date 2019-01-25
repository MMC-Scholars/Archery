

#include "ArcheryTarget.h"
#include "System/NLogger.h"


//AArcheryTarget::AArcheryTarget() {
	//m_pImg = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
//}

void AArcheryTarget::PreInit() {
	m_bActive = false;

	Msg("This better print.");
	//Msg("%d", m_pImg->GetSizeX());
	//Msg("%d", m_pImg->GetSizeY());

}

void AArcheryTarget::Activate() {
	m_bActive = true;
}

void AArcheryTarget::Deactivate() {
	m_bActive = false;

}

