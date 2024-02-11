// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInst.h"
#include "Player/PlayerCharacter.h"

void UPlayerAnimInst::UpdateValue()
{
	if (myCharacter)
		speed = myCharacter->GetVelocity().Size();
}
