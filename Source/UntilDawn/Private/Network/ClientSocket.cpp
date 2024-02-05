// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ClientSocket.h"
#include <thread>
#include "UntilDawn/UntilDawn.h"

void Send(SOCKET& sock);
void Recv(SOCKET& sock);

ClientSocket::ClientSocket()
{
	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		// ������ ����
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool ClientSocket::InitSocket()
{
	WSADATA wsaData;
	int result;
	// WinSock �ʱ�ȭ
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		ELOG(TEXT("[Error] : Failed to initialize WinSock!"));
		return false;
	}

	// ���� ����
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
	// �����ϰ��� �ϴ� ������ ���� �ּ� ����
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// ������ ���� ��û
	if (connect(clientSocket, (sockaddr*)&addr, sizeof(addr)) != 0)
	{
		ELOG(TEXT("[Error] : Failed to connect to server!"));
		return;
	}
	WLOG(TEXT("[Log] : Successfully connected to the server!"));

	return;
}

void ClientSocket::StopThread()
{
	Thread->WaitForCompletion();
	Thread->Kill();
	delete Thread;
	Thread = nullptr;
}

bool ClientSocket::Init()
{
	return true;
}

uint32 ClientSocket::Run()
{
	return 0;
}

void ClientSocket::Exit()
{
	if (clientSocket)
	{
		closesocket(clientSocket);
		WSACleanup();
		WLOG(TEXT("[Log] : Closing the connection to the server."));
	}
}

void ClientSocket::Stop()
{

}

void Send(SOCKET& sock)
{
}

void Recv(SOCKET& sock)
{
}
