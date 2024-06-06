// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerAnimInst.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UntilDawn/UntilDawn.h"

void UPlayerAnimInst::InitAnimInst(APlayerCharacter* character)
{
	myCharacter = character;
	OnMontageEnded.AddDynamic(this, &UPlayerAnimInst::MontageEnded);
}

void UPlayerAnimInst::UpdateValue()
{
	if (myCharacter)
	{
		speed = myCharacter->GetSpeed();
		isFalling = myCharacter->GetIsFalling();
		velocityZ = myCharacter->GetVelocity().Z;
		direction = myCharacter->GetDirection();
		pitch = myCharacter->GetPitch();
		yaw = myCharacter->GetYaw();
		turnRight = myCharacter->GetTurnRight();
		turnLeft = myCharacter->GetTurnLeft();
		bRightClick = myCharacter->GetRightClick();
		bAiming = myCharacter->GetAiming();
		isAbleShoot = myCharacter->GetBowStatus() ? true : false;
		currentWeaponType = myCharacter->GetCurrentWeaponType();
		isWrestling = myCharacter->GetWrestling();
	}
}

void UPlayerAnimInst::PlayWeaponArmMontage(const EWeaponType type)
{
	if (IsAnyMontagePlaying()) 
		return;
	if (type == EWeaponType::AXE && axeArmDisarmMontage)
	{
		Montage_Play(axeArmDisarmMontage);
	}
	else if (type == EWeaponType::BOW && bowArmDisarmMontage)
	{
		Montage_Play(bowArmDisarmMontage);
	}
	Montage_JumpToSection(FName("Arm"));
}

void UPlayerAnimInst::PlayWeaponDisarmMontage(const EWeaponType type)
{
	if (IsAnyMontagePlaying()) 
		return;
	if (type == EWeaponType::AXE && axeArmDisarmMontage)
	{
		Montage_Play(axeArmDisarmMontage);
	}
	else if (type == EWeaponType::BOW && bowArmDisarmMontage)
	{
		Montage_Play(bowArmDisarmMontage);
	}
	Montage_JumpToSection(FName("Disarm"));
}

void UPlayerAnimInst::PlayAxeAttackMontage()
{
	if (IsAnyMontagePlaying()) 
		return;
	if (axeAttackMontage)
	{
		Montage_Play(axeAttackMontage);
	}
}

void UPlayerAnimInst::PlayBowDrawMontage()
{
	if (IsAnyMontagePlaying()) 
		return;
	if (bowDrawMontage)
	{
		Montage_Play(bowDrawMontage);
	}
}

void UPlayerAnimInst::PlayBowShootMontage()
{
	if (IsAnyMontagePlaying()) 
		return;
	if (bowDrawMontage)
	{
		Montage_Play(bowDrawMontage);
		Montage_JumpToSection(FName("Shoot"));
	}
}

void UPlayerAnimInst::PlayLeftClickMontage(const EWeaponType type)
{
	if (type == EWeaponType::AXE)
	{
		PlayAxeAttackMontage();
	}
	else if (type == EWeaponType::BOW)
	{
		PlayBowDrawMontage();
	}
}

void UPlayerAnimInst::PlayWrestlingMontage(const bool isBlocking)
{
	if (IsAnyMontagePlaying()) 
		return;
	if (wrestlingMontage)
	{
		Montage_Play(wrestlingMontage);
		if(isBlocking)
			Montage_JumpToSection(FName("Pushing"));
	}
}

void UPlayerAnimInst::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == wrestlingMontage)
	{
		DMontageEnded.ExecuteIfBound();
	}
}
