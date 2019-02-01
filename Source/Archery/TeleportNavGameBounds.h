
#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Components/BoxComponent.h"
#include "TeleportNavGameBounds.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API ATeleportNavGameBounds : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	ATeleportNavGameBounds();
	
	UBoxComponent* m_pTrigger;
	
};
