// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ClientSocket.h"
#include "UntilDawn/UntilDawn.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

ClientSocket::ClientSocket()
{
	thread = FRunnableThread::Create(this, TEXT("Network Thread"));
}

ClientSocket::~ClientSocket()
{
	if (thread)
	{
		thread->WaitForCompletion();
		thread->Kill();
		delete thread;
		WLOG(TEXT("[Log] : Kill thread."))
	}
}

bool ClientSocket::InitSocket()
{
	//if (isInitialized)
	//	return true;

	WSADATA wsaData;
	int result;
	// WinSock 초기화
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		ELOG(TEXT("[Error] : Failed to initialize WinSock!"));
		return false;
	}

	// 소켓 생성
	clientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (clientSocket == INVALID_SOCKET)
	{
		ELOG(TEXT("[Error] : Failed to create socket!"));
		WSACleanup();
		return false;
	}
	
	return true;
}

void ClientSocket::StartSocket()
{
	//if (isInitialized)
	//	return;

	// 접속하고자 하는 서버에 대한 주소 세팅
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hello"));

	// 서버에 연결 요청
	if (connect(clientSocket, (sockaddr*)&addr, sizeof(addr)) != 0)
	{
		ELOG(TEXT("[Error] : Failed to connect to server!"));
		return;
	}

	WLOG(TEXT("[Log] : Successfully connected to the server!"));
	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Connected"));

	isInitialized = true;

	return;
}

void ClientSocket::SendAccountInfo(const FText& id, const FText& pw, const bool isLogin)
{
	std::stringstream sendStream;
	if (isLogin) sendStream << static_cast<int>(EPacketType::LOGIN) << "\n";
	else sendStream << static_cast<int>(EPacketType::SIGNUP) << "\n";
	std::string tempId(TCHAR_TO_UTF8(*id.ToString()));
	std::string tempPw(TCHAR_TO_UTF8(*pw.ToString()));
	sendStream << tempId << "\n";
	sendStream << tempPw << "\n";

	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::NotifyAccessingGame(const CharacterInfo& info)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::SPAWNPLAYER) << "\n";
	sendStream << info << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SynchronizeMyCharacterInfo(const CharacterInfo& info)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::SYNCHPLAYER) << "\n";
	sendStream << info << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendInRangeZombie(int zombieNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ZOMBIEINRANGE) << "\n";
	sendStream << zombieNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendOutRangeZombie(int zombieNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ZOMBIEOUTRANGE) << "\n";
	sendStream << zombieNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendZombieHitsMe(const int zombieNumber, const bool bResult, FHitInfo& hitInfo)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ZOMBIEHITSME) << "\n";
	sendStream << zombieNumber << "\n";
	sendStream << bResult << "\n";
	if (bResult)
	{
		sendStream << hitInfo << "\n";
	}
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendPlayerBlockingResult(const bool isSuccessToBlocking)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::WRESTLINGRESULT) << "\n";
	sendStream << isSuccessToBlocking << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendPickedItemInfo(const FString itemID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::PICKUP_ITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::UpdateItemGridPoint(const FString itemID, const int xPoint, const int yPoint, const bool isRotated)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ITEMGRIDPOINTUPDATE) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	sendStream << xPoint << "\n";
	sendStream << yPoint << "\n";
	sendStream << isRotated << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendItemInfoToEquip(const FString itemID, const int boxNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::EQUIP_ITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	sendStream << boxNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::DropEquippedItem(const FString itemID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::DROP_EQUIPPED_ITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::UnequipItem(const FString itemID, const int xPoint, const int yPoint)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::UNEQUIP_ITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	sendStream << xPoint << "\n";
	sendStream << yPoint << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::DropInventoryItem(const FString itemID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::DROP_ITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendHittedCharacters(TArray<FHitInfo>& hittedCharacters, const FString& itemID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ATTACKRESULT) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	sendStream << hittedCharacters.Num() << "\n";
	for (auto& hitInfo : hittedCharacters)
	{
		sendStream << hitInfo;
	}
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendKickedCharacters(const int characterNumber, const bool isPlayer)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::KICKEDCHARACTERS) << "\n";
	sendStream << characterNumber << "\n";
	sendStream << isPlayer << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendRespawnRequest()
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::PLAYERRESPAWN) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::ReplicateProjectile(const FVector& location, const FRotator& rotation)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::PROJECTILE) << "\n";
	sendStream << location.X << "\n" << location.Y << "\n" << location.Z << "\n";
	sendStream << rotation.Pitch << "\n" << rotation.Yaw << "\n" << rotation.Roll << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendItemUsing(const FString& itemID, const int consumedAmount)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::USINGITEM) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	sendStream << consumedAmount << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::ChangeWeapon(const FString& changedWeaponID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::CHANGE_WEAPON) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*changedWeaponID)) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::ArmWeapon(const FString& itemID)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ARM_WEAPON) << "\n";
	sendStream << std::string(TCHAR_TO_UTF8(*itemID)) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::DisarmWeapon()
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::DISARM_WEAPON) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendActivateWeaponAbility(const int32 inputType)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::ACTIVATE_WEAPON_ABILITY) << "\n";
	sendStream << static_cast<int>(inputType) << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

bool ClientSocket::Init()
{
	bRun = true;
	return true;
}

uint32 ClientSocket::Run()
{
	FPlatformProcess::Sleep(0.05);
	while (bRun)
	{
		std::stringstream recvStream;
		int recvBytes = recv(clientSocket, recvBuf, PACKET_SIZE, 0);

		if (recvBytes <= 0) break;

		recvStream << recvBuf;
		messageQ.Enqueue(std::move(recvStream));
	}
	return 0;
}

void ClientSocket::Exit()
{
	WLOG(TEXT("[Log] : Thread exit."));
}

void ClientSocket::Stop()
{
	if (!bRun) return;
	bRun = false;
	WLOG(TEXT("[Log] : Thread stopped."));
	if (clientSocket)
	{
		closesocket(clientSocket);
		WSACleanup();
		WLOG(TEXT("[Log] : Closed the connection to server."));
	}
}
