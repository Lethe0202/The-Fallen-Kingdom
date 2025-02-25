// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CheckComboInput.h"

#include "../Character/BaseCharacter.h"
#include "../Ability/AbilityComponent.h"

void UAnimNotifyState_CheckComboInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());

    if (Character != nullptr)
    {
        if (UAbilityComponent* AbilityComp = Character->GetAbilityComponent())
        {
            AbilityComp->SetEnableComboInput(true);
        }
    }
}

void UAnimNotifyState_CheckComboInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
    
    if (Character != nullptr)
    {
        if (UAbilityComponent* AbilityComp = Character->GetAbilityComponent())
        {
            AbilityComp->SetEnableComboInput(false);
            
            if (!AbilityComp->GetInputNextCombo())
            {
                Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);
            }
        }
    }
}
