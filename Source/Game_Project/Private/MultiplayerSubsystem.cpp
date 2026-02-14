// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

static const FName SESSION_NAME = FName("Test Session");
static const FName KEY_INVITE_CODE(TEXT("INVITE_CODE"));

void UMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    m_onlineSubsystem = IOnlineSubsystem::Get(TEXT("EOSPlus"));
    Login();
    //EnsureSessionInterface();
}

void UMultiplayerSubsystem::Deinitialize()
{
    //ClearSearch();
    //SessionInterface.Reset();
    Super::Deinitialize();
}

void UMultiplayerSubsystem::Login()
{
	if (m_onlineSubsystem)
	{
		if (IOnlineIdentityPtr identity = m_onlineSubsystem->GetIdentityInterface())
		{
            FOnlineAccountCredentials credentials;
            //credentials.Id = FString();
            //credentials.Token = FString();
            //credentials.Type = FString("accountportal");
            credentials.Id = FString("127.0.0.1:8081");
            credentials.Token = FString("GameProjectCred");
            credentials.Type = FString("developer");
			

            identity->OnLoginCompleteDelegates->AddUObject(this, &UMultiplayerSubsystem::HandleLoginComplete);
			identity.Get()->Login(0, credentials);
		}
	}
}

void UMultiplayerSubsystem::HandleLoginComplete(int32 a_localUserNum, bool a_bWasSuccessful, const FUniqueNetId& a_userId, const FString& a_error)
{
	UE_LOG(LogTemp, Warning, TEXT("Logged In: %d"), a_bWasSuccessful);

    m_isLoggedIn = a_bWasSuccessful;

    if (m_onlineSubsystem)
    {
        if (IOnlineIdentityPtr identity = m_onlineSubsystem->GetIdentityInterface())
        {
            FName subsystemName = m_onlineSubsystem->GetSubsystemName();
            FText serviceName = m_onlineSubsystem->GetOnlineServiceName();
            UE_LOG(LogTemp, Warning, TEXT("Subsystem Name: %s"), *subsystemName.ToString());
            UE_LOG(LogTemp, Warning, TEXT("Service Name: %s"), *serviceName.ToString());

            identity->ClearOnLoginCompleteDelegates(0, this);
        }
    }
}

void UMultiplayerSubsystem::CreateSession()
{
    if (m_isLoggedIn)
    {
        TSharedPtr<const FUniqueNetId> userId = m_onlineSubsystem->GetIdentityInterface()->GetUniquePlayerId(0);
        if (!userId.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("No Valid UserID!"));
	        return;
        }
        if (m_onlineSubsystem)
        {
            if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
            {
                UE_LOG(LogTemp, Warning, TEXT("Create Session with PUID: %s"), *userId->ToString());

                m_lastHostedLobbyCode = MakeLobbyCode(6);

                UE_LOG(LogTemp, Warning, TEXT("Session Code: %s"), *m_lastHostedLobbyCode);

                if (sessionPtr->GetNumSessions() > 0)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Currently running: %i Sessions trying to destroy it first"), sessionPtr->GetNumSessions());

                    DestroySessionForNewOne();
                }
                else
                {
                    FOnlineSessionSettings sessionSettings;
                    sessionSettings.bIsDedicated = false;
                    sessionSettings.bShouldAdvertise = true;
                    sessionSettings.bIsLANMatch = false;
                    sessionSettings.NumPublicConnections = 4;
                    sessionSettings.bAllowJoinInProgress = true;
                    sessionSettings.bAllowJoinViaPresence = true;
                    sessionSettings.bUsesPresence = true;
                    sessionSettings.bUseLobbiesIfAvailable = true;
                    sessionSettings.Set(SEARCH_KEYWORDS, m_lastHostedLobbyCode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

                    sessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::HandleCreateSessionComplete);
                    sessionPtr->CreateSession(0, SESSION_NAME, sessionSettings);
                }
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create Session: User not logged in"));
    }
}

void UMultiplayerSubsystem::HandleCreateSessionComplete(FName a_sessionName, bool a_bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("Session Created Successfull: %d"), a_bWasSuccessful);

    if (m_onlineSubsystem)
    {
        if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
        {
            if (a_bWasSuccessful)
            {
	            OnLobbyCreateSuccess.Broadcast();
            }
            sessionPtr->ClearOnCreateSessionCompleteDelegates(this);
            
            //GetWorld()->ServerTravel("/Game/MainHub1?listen");
            UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/MainHub1"), true, TEXT("listen"));
        }
    }
}

void UMultiplayerSubsystem::DestroySession()
{
    if (m_isLoggedIn)
    {
        if (m_onlineSubsystem)
        {
            if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
            {
                sessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::HandleDestroySessionComplete);
                sessionPtr->DestroySession(SESSION_NAME);
            }
        }
    }
}


void UMultiplayerSubsystem::HandleDestroySessionComplete(FName a_sessionName, bool a_bWasSuccessful)
{
    if (m_onlineSubsystem)
    {
        if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
        {
            if (a_bWasSuccessful)
            {
                UE_LOG(LogTemp, Warning, TEXT("Session Destroyed"));
            }
            sessionPtr->ClearOnDestroySessionCompleteDelegates(this);
        }
    }
}

void UMultiplayerSubsystem::DestroySessionForNewOne()
{
    if (m_isLoggedIn)
    {
        if (m_onlineSubsystem)
        {
            if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
            {
                sessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::HandleDestroySessionForNewOneComplete);
                sessionPtr->DestroySession(SESSION_NAME);
            }
        }
    }
}

void UMultiplayerSubsystem::HandleDestroySessionForNewOneComplete(FName a_sessionName, bool a_bWasSuccessful)
{
    if (m_onlineSubsystem)
    {
        if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
        {
            if (a_bWasSuccessful)
            {
                UE_LOG(LogTemp, Warning, TEXT("Session Destroyed trying to create new Session"));
            }
            sessionPtr->ClearOnDestroySessionCompleteDelegates(this);
            CreateSession();
        }
    }
}


void UMultiplayerSubsystem::GetAllFriends()
{
    if (m_isLoggedIn)
    {
        if (m_onlineSubsystem)
        {
            if (IOnlineFriendsPtr friendsPtr = m_onlineSubsystem->GetFriendsInterface())
            {
                friendsPtr->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UMultiplayerSubsystem::OnGetAllFriendsComplete));
            }
        }
    }
}

void UMultiplayerSubsystem::OnGetAllFriendsComplete(int32 a_localUserNum, bool a_bWasSuccessful, const FString& a_listName, const FString& a_errorStr)
{
	if (a_bWasSuccessful)
	{
        if (m_onlineSubsystem)
        {
	        if (IOnlineFriendsPtr friendsPtr = m_onlineSubsystem->GetFriendsInterface())
	        {
                TArray<TSharedRef<FOnlineFriend>> friendsList;
		        if (friendsPtr->GetFriendsList(0, a_listName, friendsList))
		        {
			        for (TSharedRef<FOnlineFriend> onlineFriend : friendsList)
			        {
				        FString FriendName = onlineFriend.Get().GetRealName();
                        UE_LOG(LogTemp, Warning, TEXT("Friend: %s"), *FriendName);
			        }
			        
		        }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed Getting Friends List"));
                }
	        }
        }
		UE_LOG(LogTemp, Warning, TEXT("Successful Got Friends List"));
	}
}

void UMultiplayerSubsystem::ShowInviteUI()
{
    if (m_isLoggedIn)
    {
        if (m_onlineSubsystem)
        {
            if (IOnlineExternalUIPtr uiPtr = m_onlineSubsystem->GetExternalUIInterface())
            {
                uiPtr->ShowInviteUI(0,SESSION_NAME);
            }
        }
    }
}

void UMultiplayerSubsystem::FindSessions(FString a_lobbyCode)
{
    if (m_isLoggedIn)
    {
        if (m_onlineSubsystem)
        {
            if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
            {
                m_searchSettings = MakeShareable(new FOnlineSessionSearch());
                m_searchSettings->MaxSearchResults = 5000;
                m_searchSettings->QuerySettings.Set(SEARCH_KEYWORDS, a_lobbyCode, EOnlineComparisonOp::Equals);
                m_searchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
                sessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::HandleFindSessionsComplete);
                sessionPtr->FindSessions(0, m_searchSettings.ToSharedRef());
            }
        }
    }
}

void UMultiplayerSubsystem::HandleFindSessionsComplete(bool a_bWasSuccessful)
{
    UE_LOG(LogTemp, Warning, TEXT("LobbySearch Successful: %d"), a_bWasSuccessful);

	if (a_bWasSuccessful)
	{
        UE_LOG(LogTemp, Warning, TEXT("Found %d lobbies"), m_searchSettings->SearchResults.Num());
        if (m_onlineSubsystem)
        {
            if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
            {
                if (m_searchSettings->SearchResults.Num())
                {
                    sessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSubsystem::HandleJoinSessionComplete);
                    sessionPtr->JoinSession(0, SESSION_NAME, m_searchSettings->SearchResults[0]);
                }
            }
        }
	}

    if (m_onlineSubsystem)
    {
        if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
        {
            sessionPtr->ClearOnFindSessionsCompleteDelegates(this);
        }
    }
}

void UMultiplayerSubsystem::HandleJoinSessionComplete(FName a_sessionName, EOnJoinSessionCompleteResult::Type a_result)
{
    if (m_onlineSubsystem)
    {
        if (IOnlineSessionPtr sessionPtr = m_onlineSubsystem->GetSessionInterface())
        {
            FString connectionInfo = FString();
            sessionPtr->GetResolvedConnectString(a_sessionName, connectionInfo);
            if (!connectionInfo.IsEmpty())
            {
	            if (APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	            {
		            pc->ClientTravel(connectionInfo, TRAVEL_Absolute);
	            }
            }
        }
    }
}




FString UMultiplayerSubsystem::MakeLobbyCode(int32 Len) const
{
    // Avoid confusing chars: 0/O, 1/I
    const FString Alphabet = TEXT("ABCDEFGHJKLMNPQRSTUVWXYZ23456789");
    FString Out;
    Out.Reserve(Len);

    for (int32 i = 0; i < Len; i++)
    {
        const int32 Idx = FMath::RandRange(0, Alphabet.Len() - 1);
        Out.AppendChar(Alphabet[Idx]);
    }
    return Out;
}

//void UMultiplayerSubsystem::CreateSession(const FString& LobbyMapName)
//{
//    EnsureSessionInterface();
//    if (!SessionInterface.IsValid())
//    {
//        OnLobbyJoinError.Broadcast(TEXT("No SessionInterface (OnlineSubsystem nicht initialisiert)."));
//        return;
//    }
//
//    PendingLobbyMapName = LobbyMapName;
//
//    // Wenn schon eine Session existiert: vorher zerstören
//    if (SessionInterface->GetNamedSession(SESSION_NAME))
//    {
//        DestroyHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
//            FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::HandleDestroySessionComplete)
//        );
//        SessionInterface->DestroySession(SESSION_NAME);
//        return;
//    }
//
//    // Create directly
//    LastHostedLobbyCode = MakeLobbyCode(6);
//
//    FOnlineSessionSettings Settings;
//    Settings.NumPublicConnections = 4;
//    Settings.NumPrivateConnections = 0;
//
//    Settings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
//    Settings.bShouldAdvertise = true;
//    Settings.bAllowJoinInProgress = true;
//    Settings.bAllowJoinViaPresence = true;
//
//    // “Lobby-ish” sessions typically rely on presence
//    Settings.bUsesPresence = true;
//
//    // This helps on some OSS implementations that support lobbies
//    Settings.bUseLobbiesIfAvailable = true;
//
//    // Publish the lobby code as a searchable attribute
//    Settings.Set(KEY_INVITE_CODE, LastHostedLobbyCode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
//
//    Settings.BuildUniqueId = 1;
//
//    CreateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
//        FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::HandleCreateSessionComplete)
//    );
//
//    //const int32 LocalUserNum = 0;
//    SessionInterface->CreateSession(GetWorld()->GetFirstLocalPlayerFromController()->GetLocalPlayerIndex(), NAME_GameSession, Settings);
//}

//void UMultiplayerSubsystem::HandleDestroySessionComplete(FName SessionName, bool bWasSuccessful)
//{
//    if (SessionInterface.IsValid() && DestroyHandle.IsValid())
//    {
//        SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroyHandle);
//        DestroyHandle.Reset();
//    }
//
//    // Try again
//    CreateSession(PendingLobbyMapName);
//}

//void UMultiplayerSubsystem::HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful)
//{
//    UE_LOG(LogTemp, Error, TEXT("Session successfully created: %d"), bWasSuccessful);
//
//    if (SessionInterface.IsValid() && CreateHandle.IsValid())
//    {
//        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateHandle);
//        CreateHandle.Reset();
//    }
//
//    if (!bWasSuccessful)
//    {
//        OnLobbyJoinError.Broadcast(TEXT("CreateSession fehlgeschlagen."));
//        return;
//    }
//
//    OnLobbyCodeCreated.Broadcast(LastHostedLobbyCode);
//
//    // Host travels to lobby as listen server
//    UGameplayStatics::OpenLevel(GetWorld(), FName(*PendingLobbyMapName), true, TEXT("listen"));
//
//    shouldOpen = true;
//
//    UE_LOG(LogTemp,Warning, TEXT("Lobby Code: %s"), *LastHostedLobbyCode);
//}

//void UMultiplayerSubsystem::JoinLobbyByCode(const FString& LobbyCode)
//{
//    EnsureSessionInterface();
//    if (!SessionInterface.IsValid())
//    {
//        OnLobbyJoinError.Broadcast(TEXT("No SessionInterface (OnlineSubsystem nicht initialisiert)."));
//        return;
//    }
//
//    if (LobbyCode.IsEmpty())
//    {
//        OnLobbyJoinError.Broadcast(TEXT("Lobby-Code ist leer."));
//        return;
//    }
//
//    SessionSearch = MakeShared<FOnlineSessionSearch>();
//    SessionSearch->MaxSearchResults = 25;
//    SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
//
//    // Presence sessions
//    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
//
//    // Filter by invite code
//    SessionSearch->QuerySettings.Set(KEY_INVITE_CODE, LobbyCode, EOnlineComparisonOp::Equals);
//
//    OnLobbySearchStatus.Broadcast(true);
//
//    FindHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
//        FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::HandleFindSessionsComplete)
//    );
//
//    //const int32 LocalUserNum = 0;
//    SessionInterface->FindSessions(GetWorld()->GetFirstLocalPlayerFromController()->GetLocalPlayerIndex(), SessionSearch.ToSharedRef());
//}

//void UMultiplayerSubsystem::HandleFindSessionsComplete(bool bWasSuccessful)
//{
//    OnLobbySearchStatus.Broadcast(false);
//
//    if (SessionInterface.IsValid() && FindHandle.IsValid())
//    {
//        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindHandle);
//        FindHandle.Reset();
//    }
//
//    if (!bWasSuccessful || !SessionSearch.IsValid())
//    {
//        OnLobbyJoinError.Broadcast(TEXT("FindSessions fehlgeschlagen."));
//        ClearSearch();
//        return;
//    }
//
//    if (SessionSearch->SearchResults.Num() < 1)
//    {
//        OnLobbyJoinError.Broadcast(TEXT("Keine Lobby mit diesem Code gefunden."));
//        ClearSearch();
//        return;
//    }
//
//    // Take first match (code should be unique)
//    const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[0];
//
//    JoinHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
//        FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::HandleJoinSessionComplete)
//    );
//
//    const int32 LocalUserNum = 0;
//    SessionInterface->JoinSession(LocalUserNum, SESSION_NAME, Result);
//}

//void UMultiplayerSubsystem::HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//    if (SessionInterface.IsValid() && JoinHandle.IsValid())
//    {
//        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinHandle);
//        JoinHandle.Reset();
//    }
//
//    if (Result != EOnJoinSessionCompleteResult::Success)
//    {
//        OnLobbyJoinError.Broadcast(TEXT("JoinSession fehlgeschlagen."));
//        ClearSearch();
//        return;
//    }
//
//    FString ConnectString;
//    if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
//    {
//        OnLobbyJoinError.Broadcast(TEXT("Konnte ConnectString nicht auflösen."));
//        ClearSearch();
//        return;
//    }
//
//    if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
//    {
//        OnLobbyJoinSuccess.Broadcast();
//        PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
//    }
//    else
//    {
//        OnLobbyJoinError.Broadcast(TEXT("Kein PlayerController gefunden."));
//    }
//
//    ClearSearch();
//}

//void UMultiplayerSubsystem::LeaveLobby()
//{
//    EnsureSessionInterface();
//    if (!SessionInterface.IsValid())
//        return;
//
//    if (!SessionInterface->GetNamedSession(SESSION_NAME))
//        return;
//
//    DestroyHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
//        FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMultiplayerSubsystem::HandleDestroySessionComplete)
//    );
//    SessionInterface->DestroySession(SESSION_NAME);
//}
//
//void UMultiplayerSubsystem::ClearSearch()
//{
//    SessionSearch.Reset();
//}

FName UMultiplayerSubsystem::GetOnlineSubsystemName() const
{
	return IOnlineSubsystem::Get()->GetSubsystemName();
}

FString UMultiplayerSubsystem::GetLobbyCode() const
{
	return m_lastHostedLobbyCode;
}
