// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Main/WidgetWrestlingProgress.h"
#include "Components/ProgressBar.h"

bool UWidgetWrestlingProgress::ReducingProgressBar(float deltaTime)
{
	const float percent = FMath::Max(ResistProgressBar->GetPercent() - (deltaTime * 0.1f), 0.f);
	ResistProgressBar->SetPercent(percent);
	return percent <= 0.f;
}

bool UWidgetWrestlingProgress::IncreasingProgressBar()
{
	PlayAnimation(ButtonAction, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, true);
	const float percent = FMath::Min(ResistProgressBar->GetPercent() + (0.075f), 1.f);
	ResistProgressBar->SetPercent(percent);
	return percent >= 1.f;
}

void UWidgetWrestlingProgress::ResetProgressBar()
{
	StopAnimation(HighlightText);
	ResistProgressBar->SetPercent(0.5f);
}

void UWidgetWrestlingProgress::PlayHighlightTextAnimation()
{
	PlayAnimation(HighlightText, 0.f, 0, EUMGSequencePlayMode::Forward, 1.f, true);
}