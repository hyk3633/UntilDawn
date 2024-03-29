﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ClientSocket.h"
#include "UntilDawn/UntilDawn.h"
#include "GameMode/GameModeMainMap.h"
#include "Player/Login/PlayerControllerLoginMap.h"

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
	if (isInitialized)
		return true;

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
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
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
	if (isInitialized)
		return;

	// 접속하고자 하는 서버에 대한 주소 세팅
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// 서버에 연결 요청
	if (connect(clientSocket, (sockaddr*)&addr, sizeof(addr)) != 0)
	{
		ELOG(TEXT("[Error] : Failed to connect to server!"));
		return;
	}
	WLOG(TEXT("[Log] : Successfully connected to the server!"));

	isInitialized = true;

	return;
}

void ClientSocket::Recv(std::stringstream& recvStream)
{
	
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

void ClientSocket::SynchronizeMyCharacterInfo(const PlayerInfo& info)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::SYNCHPLAYER) << "\n";
	sendStream << info << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendPlayerInputAction(const int inputType)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::PLAYERINPUTACTION) << "\n";
	sendStream << inputType << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendPlayerBlockingResult(const bool isSuccessToBlocking)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::WRESTLINGRESULT) << "\n";
	sendStream << isSuccessToBlocking << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendPickedItemInfo(const int itemNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::SYNCHITEM) << "\n";
	sendStream << itemNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendHitPlayerInfo(const int playerNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::HITPLAYER) << "\n";
	sendStream << playerNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendHitZombieInfo(const int zombieNumber)
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::HITZOMBIE) << "\n";
	sendStream << zombieNumber << "\n";
	send(clientSocket, (CHAR*)sendStream.str().c_str(), sendStream.str().length(), 0);
}

void ClientSocket::SendRespawnRequest()
{
	std::stringstream sendStream;
	sendStream << static_cast<int>(EPacketType::PLAYERRESPAWN) << "\n";
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

		int packetType;
		if (recvBytes > 0)
		{
			recvStream << recvBuf;
			recvStream >> packetType;
			ProcessPacket(static_cast<EPacketType>(packetType), recvStream);
		}
	}
	return 0;
}

void ClientSocket::ProcessPacket(const EPacketType type, std::stringstream& recvStream)
{
	switch (type)
	{
		case EPacketType::SIGNUP:
		{
			bool isGranted;
			recvStream >> isGranted;
			if(ownerController)
				ownerController->ReceiveSignUpRequestResult(isGranted);
			break;
		}
		case EPacketType::LOGIN:
		{
			bool isGranted;
			recvStream >> isGranted;
			int playerNumber = -1;
			if (isGranted)
			{
				recvStream >> playerNumber;
			}
			if (ownerController)
				ownerController->ReceiveLoginRequestResult(isGranted, playerNumber);
			break;
		}
		case EPacketType::SPAWNPLAYER:
		{
			newPlayerInfoSetEx.InputStreamWithID(recvStream);
			if (ownerGameMode)
				ownerGameMode->ReceiveNewPlayerInfo(&newPlayerInfoSetEx);
			break;
		}
		case EPacketType::SYNCHPLAYER:
		{
			recvStream >> synchPlayerInfoSet;
			if (ownerGameMode)
				ownerGameMode->ReceiveOtherPlayersInfo(&synchPlayerInfoSet);
			break;
		}
		case EPacketType::PLAYERINPUTACTION:
		{
			int number = 0, inputType = 0;
			recvStream >> number >> inputType;
			if (ownerGameMode)
				ownerGameMode->SynchronizeOtherPlayerInputAction(number, inputType);
			break;
		}
		case EPacketType::WRESTLINGRESULT:
		{
			bool wrestlingResult = false;
			int number = 0;
			recvStream >> number >> wrestlingResult;
			if (ownerGameMode)
				ownerGameMode->PlayWrestlingResultAction(number, wrestlingResult);
			break;
		}
		case EPacketType::WRESTLINGSTART:
		{
			int number = 0;
			recvStream >> number;
			if (ownerGameMode)
				ownerGameMode->ReceiveWrestlingPlayer(number);
			break;
		}
		case EPacketType::SYNCHITEM:
		{
			recvStream >> synchItemInfoSet;
			if (ownerGameMode)
				ownerGameMode->ReceiveItemInfo(&synchItemInfoSet);
			break;
		}
		case EPacketType::PLAYERRESPAWN:
		{
			int number = 0;
			CharacterInfo respawnInfo;
			recvStream >> number >> respawnInfo;
			if (ownerGameMode)
				ownerGameMode->ReceiveRespawnPlayerNumber(number, respawnInfo);
			break;
		}
		case EPacketType::INITIALINFO:
		{
			while (1)
			{
				int type2 = -1;
				recvStream >> type2;
				if (type2 == -1) break;
				ProcessPacket(static_cast<EPacketType>(type2), recvStream);
			}
			break;
		}
		case EPacketType::SYNCHZOMBIE:
		{
			recvStream >> synchZombieInfoSet;
			if (ownerGameMode)
				ownerGameMode->ReceiveZombieInfo(&synchZombieInfoSet);
			break;
		}
		case EPacketType::PLAYERDISCONNECTED:
		{
			int number = 0;
			recvStream >> number;
			if (ownerGameMode)
				ownerGameMode->ReceiveDisconnectedPlayerInfo(number);
			break;
		}
		case EPacketType::DESTROYITEM:
		{
			int itemNumber = 0;
			recvStream >> itemNumber;
			if (ownerGameMode)
				ownerGameMode->DestroyItem(itemNumber);
			break;
		}
		case EPacketType::PICKUPITEM:
		{
			int itemNumber = 0;
			recvStream >> itemNumber;
			if (ownerGameMode)
				ownerGameMode->PickUpItem(itemNumber);
			break;
		}
		case EPacketType::ZOMBIEDEAD:
		{
			int zombieNumber = 0;
			recvStream >> zombieNumber;
			if (ownerGameMode)
				ownerGameMode->ReceiveDeadZombieNumber(zombieNumber);
			break;
		}
		case EPacketType::PLAYERDEAD:
		{
			int playerNumber = 0;
			recvStream >> playerNumber;
			if (ownerGameMode)
				ownerGameMode->ReceiveDeadPlayerNumber(playerNumber);
			break;
		}
	}
}

void ClientSocket::Exit()
{
	WLOG(TEXT("exit?"));
}

void ClientSocket::Stop()
{
	if (!bRun) return;
	bRun = false;
	WLOG(TEXT("[Log] : Stop thread."));
	if (clientSocket)
	{
		closesocket(clientSocket);
		WSACleanup();
		WLOG(TEXT("[Log] : Closing the connection to the server."));
	}
}
