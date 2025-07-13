// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"

void UMyGameInstance::CreateSession()
{
	IOnlineSessionPtr onlineSession = Online::GetSessionInterface(GetWorld());

	if (onlineSession.IsValid())
	{
		if (OnlineSessionPtr)
		{
			OnlineSessionPtr->DestroySession(NAME_GameSession);
		}

		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(FName("VOVA"), FString("TRUE"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		onlineSession->CreateSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(),
									 NAME_GameSession, SessionSettings);
	}
}

void UMyGameInstance::FindSession()
{
	if (OnlineSessionPtr.IsValid())
	{
		OnlineSessionSearch->bIsLanQuery = false;
		OnlineSessionSearch->MaxSearchResults = 1000;
		OnlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		OnlineSessionPtr->FindSessions(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(),
									   OnlineSessionSearch.ToSharedRef());
	}
}

void UMyGameInstance::OnSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
                                              FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	FString sessionId = InviteResult.GetSessionIdStr();
	FString userName = InviteResult.Session.OwningUserName;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Emerald,
									 FString::Printf(
										 TEXT("ControllerId = %d, UserId = %s, sessionId = %s, host_name = %s"), ControllerId,
										 *UserId.Get()->ToString(), *sessionId, *userName));
	
	OnlineSessionPtr->JoinSession(ControllerId, NAME_GameSession, InviteResult);
}

void UMyGameInstance::OnSessionCreated(FName SessionName, bool IsWasCreated)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,
									 FString::Printf(
										 TEXT("In Session Name = %s, It was created = %d"), *SessionName.ToString(),
										 IsWasCreated));
	GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/SessionLevel?listen"), true);
}

void UMyGameInstance::OnSessionFound(bool IsWasFound)
{
	if (!IsWasFound || !OnlineSessionSearch.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Can`t found sessions")));
		return;
	}


	for (auto result : OnlineSessionSearch->SearchResults)
	{
		FString sessionId = result.GetSessionIdStr();
		FString userName = result.Session.OwningUserName;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue,
										 FString::Printf(
											 TEXT("sessionId = %s, host_name = %s"), *sessionId, *userName));

		FString isVovaGay;
		result.Session.SessionSettings.Get(FName(TEXT("VOVA")), isVovaGay);
		if (isVovaGay == TEXT("TRUE"))
		{
			OnlineSessionPtr->JoinSession(*GetWorld()->GetFirstLocalPlayerFromController()->GetPreferredUniqueNetId(),
										  NAME_GameSession, result);
			return;
		}
	}
}

void UMyGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type SessionResult)
{
	if (SessionResult == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			FString JoinAddress;
			if (OnlineSessionPtr->GetResolvedConnectString(SessionName, JoinAddress))
			{
				PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session."));
	}
}

void UMyGameInstance::CreateOnlineSessionPtr()
{
	if (OnlineSessionSearch)
		return;
	
	OnlineSessionSearch = MakeShared<FOnlineSessionSearch>();
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
										 FString::Printf(TEXT("%s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		OnlineSessionPtr = OnlineSubsystem->GetSessionInterface();
		
		OnlineSessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnSessionCreated);
		OnlineSessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this,  &UMyGameInstance::OnJoinSession);
		OnlineSessionPtr->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &UMyGameInstance::OnSessionInviteAccepted);
		OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnSessionFound);
	}
}
