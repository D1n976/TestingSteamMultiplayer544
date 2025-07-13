// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEAMTEST544_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void CreateOnlineSessionPtr();
	
	void CreateSession();
	void FindSession();
private:
	void OnSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);	
	void OnSessionCreated(FName SessionName, bool IsWasCreated);
	void OnSessionFound(bool IsWasFound);
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type SessionResult);
	
	IOnlineSessionPtr OnlineSessionPtr;
	
	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	
};
