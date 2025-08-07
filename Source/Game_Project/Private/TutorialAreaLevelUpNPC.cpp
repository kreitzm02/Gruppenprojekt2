// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialAreaLevelUpNPC.h"

void ATutorialAreaLevelUpNPC::Interact()
{
    if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
    {
        APlayerCharacter* player = Cast<APlayerCharacter>(pc->GetCharacter());

        player->AddExperiencePoints(250);
    }
    //UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
}