

#include "LevelMain.h"
#include "System/NLogger.h"
#include "Archery.h"
/*
#include "TeleportNavGameBounds.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
*/
ALevelMain::ALevelMain() {
	g_archeryGlobals.m_iScore = 0;

	m_pScore = CreateDefaultSubobject<UTextRenderComponent>("Score Tracker");
	m_pScore->SetTextRenderColor(FColor::White);
	m_pScore->SetXScale(1.0f);
	m_pScore->SetYScale(1.0f);
	m_pScore->SetHorizontalAlignment(EHorizTextAligment::EHTA_Left);
}

void ALevelMain::PostInit() {
	//
	//
	//
	//TODO - move when available on game start
	//TEMP
	// ON GAME START. MOVE THIS WHEN AVAILABLE
	/*
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeleportNavGameBounds::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++) {
	ATeleportNavGameBounds* bounds = Cast<ATeleportNavGameBounds>(FoundActors[i]);
	if (bounds) {
	g_pBasePawn->m_pTeleportBounds = bounds;
	break;
	}
	}
	*/

	//
	//
	//
}

void ALevelMain::DefaultThink() {
	SetScore(g_archeryGlobals.m_iScore);
}

void ALevelMain::SetScore(int score) {

	char buffer[50];

	snprintf(buffer, sizeof(buffer), "%d", score);

	char str[100];
	
	strcpy_s(str, "Score: ");
	strcat_s(str, buffer);
	strcat_s(str, "\nTime: ");
	strcat_s(str, buffer);

	m_pScore->SetText(FText::AsCultureInvariant(str));
}