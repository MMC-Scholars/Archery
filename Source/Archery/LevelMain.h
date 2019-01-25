

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/TextRenderComponent.h"
#include "LevelMain.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API ALevelMain : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	// Constructor
	ALevelMain();

	UTextRenderComponent* m_pScore;
	unsigned int m_iScore;

	void PostInit();

	void DefaultThink();

};
