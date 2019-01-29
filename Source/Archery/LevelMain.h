

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/TextRenderComponent.h"
#include <string>
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

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Score Text")
		UTextRenderComponent* m_pScore;

	// Functions

	void PostInit();

	void DefaultThink();

	void SetScore(int score);

};
