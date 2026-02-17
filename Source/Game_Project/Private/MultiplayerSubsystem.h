// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPSessionInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MultiplayerSubsystem.generated.h"



//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCodeCreated, const FString&, LobbyCode);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyJoinError, const FString&, Error);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyJoinSuccess);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbySearchStatus, bool, bSearching);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyCreateSuccess);

/**
 * 
 */
UCLASS(Blueprintable)
class UMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;


    // Call on Host
    UFUNCTION(BlueprintCallable)
    void CreateSession();

    // Call on Host Leave Session
    UFUNCTION(BlueprintCallable)
    void DestroySession();

    UFUNCTION(BlueprintCallable)
    void DestroySessionForNewOne();

    // Call to get all Friends in Friendslist
    UFUNCTION(BlueprintCallable)
    void GetAllFriends();

    //Can be called in session to invite
    UFUNCTION(BlueprintCallable)
    void ShowInviteUI();

    //Call to search for lobbies via lobby code
    UFUNCTION(BlueprintCallable)
    void FindSessions(FString a_lobbyCode);
    TSharedPtr<FOnlineSessionSearch> m_searchSettings;

    //// Join flow
    //UFUNCTION(BlueprintCallable)
    //void JoinLobbyByCode(const FString& LobbyCode);
    //
    //// Optional: call to leave/destroy session (host/client)
    //UFUNCTION(BlueprintCallable)
    //void LeaveLobby();
    
    
    

    // Events for UI
    UPROPERTY(BlueprintAssignable) FOnLobbyCreateSuccess OnLobbyCreateSuccess;
    
    //UFUNCTION(BlueprintPure)
    //FString GetLastHostedLobbyCode() const { return LastHostedLobbyCode; }


private:
    IOnlineSubsystem* m_onlineSubsystem;

    bool m_isLoggedIn = false;

    FString m_lastHostedLobbyCode;

    void Login();

    //// Delegates
	void HandleLoginComplete(int32 a_localUserNum, bool a_bWasSuccessful, const FUniqueNetId& a_userId, const FString& a_error);
    void HandleCreateSessionComplete(FName a_sessionName, bool a_bWasSuccessful);
    void HandleDestroySessionComplete(FName a_sessionName, bool a_bWasSuccessful);
    void HandleDestroySessionForNewOneComplete(FName a_sessionName, bool a_bWasSuccessful);
    void OnGetAllFriendsComplete(int32 a_localUserNum, bool a_bWasSuccessful, const FString& a_listName, const FString& a_errorStr);
    void HandleFindSessionsComplete(bool a_bWasSuccessful);
    void HandleJoinSessionComplete(FName a_sessionName, EOnJoinSessionCompleteResult::Type a_result);


	// Helpers
    FString MakeLobbyCode(int32 Len = 6) const;
    //void ClearSearch();

    UFUNCTION(BlueprintCallable)
    FName GetOnlineSubsystemName() const;

    UFUNCTION(BlueprintCallable)
    FString GetLobbyCode() const;
};
