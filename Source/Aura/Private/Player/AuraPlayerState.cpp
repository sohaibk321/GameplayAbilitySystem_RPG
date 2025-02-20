// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// how often the server will try to update clients; higher number = checks for updates more often
	NetUpdateFrequency = 100.f; 
}
