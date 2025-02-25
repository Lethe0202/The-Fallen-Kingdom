// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AdditionalAction.h"
#include "../../Character/BaseCharacter.h"
#include "../Ability/AbilityComponent.h"


void UAnimNotify_AdditionalAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (BaseCharacter != nullptr)
	{
		UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>();

		AbilityComponent->AbilityAdditionalAction(AbilityClass, ActionIndex);
	}
}
