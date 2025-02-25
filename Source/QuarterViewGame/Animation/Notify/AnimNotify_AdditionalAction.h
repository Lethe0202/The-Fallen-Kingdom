// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AdditionalAction.generated.h"

/**
 * 애니메이션 몽타주 실행 중 캐릭터가 추가적인 행동이 필요할때
 */
UCLASS()
class QUARTERVIEWGAME_API UAnimNotify_AdditionalAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Ability")
	TSubclassOf<class UBaseAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Ability")
	int32 ActionIndex;
};
