#pragma once
#include "GameplayTagContainer.h"

#define UD_CHARACTER_STATE_AIMING FGameplayTag::RequestGameplayTag(FName("Character.State.Aiming"))
#define UD_CHARACTER_STATE_HITREACTION FGameplayTag::RequestGameplayTag(FName("Character.State.HitReaction"))
#define UD_CHARACTER_STATE_KICKED FGameplayTag::RequestGameplayTag(FName("Character.State.Kicked"))
#define UD_EVENT_CHARACTER_HITREACTION FGameplayTag::RequestGameplayTag(FName("Event.Character.HitReaction"))
#define UD_EVENT_CHARACTER_CHANGEWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.ChangeWeapon"))
#define UD_EVENT_CHARACTER_ARMWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.ArmWeapon"))
#define UD_EVENT_CHARACTER_DISARMWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.DisarmWeapon"))