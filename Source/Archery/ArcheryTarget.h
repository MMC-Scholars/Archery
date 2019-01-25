

#pragma once

#include "CoreMinimal.h"
#include "VRBase/ABaseEntity/ABaseEntity.h"
#include "Engine/Texture2D.h"
#include "ArcheryTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERY_API AArcheryTarget : public ABaseEntity
{
	GENERATED_BODY()
	
public:
	// Constructor
	//AArcheryTarget();

	// Variables
	bool m_bActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target Image")
	UTexture2D* m_pImg;
	
	// Functions
	void PreInit() override;

	void Activate();
	void Deactivate();

	
// set image dynamically from within the editor
#if WITH_EDITOR
virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
	? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (m_pImg != nullptr) {}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


};
