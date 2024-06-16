#pragma once
#include "GameplayTagContainer.h"

#define UD_CHARACTER_STATE_AIMING FGameplayTag::RequestGameplayTag(FName("Character.State.Aiming"))
#define UD_CHARACTER_STATE_HITREACTION FGameplayTag::RequestGameplayTag(FName("Character.State.HitReaction"))
#define UD_CHARACTER_STATE_KICKED FGameplayTag::RequestGameplayTag(FName("Character.State.Kicked"))
#define UD_CHARACTER_STATE_WRESTLING FGameplayTag::RequestGameplayTag(FName("Character.State.Wrestling"))
#define UD_CHARACTER_STATE_BLOCKSTAMINARECOVERY FGameplayTag::RequestGameplayTag(FName("Character.State.BlockStaminaRecovery"))

#define UD_EVENT_CHARACTER_HITREACTION FGameplayTag::RequestGameplayTag(FName("Event.Character.HitReaction"))
#define UD_EVENT_CHARACTER_KICKREACTION FGameplayTag::RequestGameplayTag(FName("Event.Character.KickReaction"))
#define UD_EVENT_CHARACTER_HITCHECK FGameplayTag::RequestGameplayTag(FName("Event.Character.HitCheck"))
#define UD_EVENT_CHARACTER_CHANGEWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.ChangeWeapon"))
#define UD_EVENT_CHARACTER_ARMWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.ArmWeapon"))
#define UD_EVENT_CHARACTER_DISARMWEAPON FGameplayTag::RequestGameplayTag(FName("Event.Character.DisarmWeapon"))
#define UD_EVENT_CHARACTER_KICK FGameplayTag::RequestGameplayTag(FName("Event.Character.Kick"))
#define UD_EVENT_CHARACTER_HITBYZOMBIE FGameplayTag::RequestGameplayTag(FName("Event.Character.HitByZombie"))

#define UD_EVENT_SEND_HITREACTION FGameplayTag::RequestGameplayTag(FName("Event.Send.HitReaction"))
#define UD_EVENT_SEND_KICK FGameplayTag::RequestGameplayTag(FName("Event.Send.Kick"))

#define UD_EVENT_WRESTLING_START FGameplayTag::RequestGameplayTag(FName("Event.Wrestling.Start"))
#define UD_EVENT_WRESTLING_BLOCKED FGameplayTag::RequestGameplayTag(FName("Event.Wrestling.Blocked"))
#define UD_EVENT_WRESTLING_BITED FGameplayTag::RequestGameplayTag(FName("Event.Wrestling.Bited"))

#define UD_DATA_STAMINA FGameplayTag::RequestGameplayTag(FName("Data.Stamina"))

#define UD_GAMEPLAYCUE_CAMERASHAKE_HIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.CameraShake.Hit"))