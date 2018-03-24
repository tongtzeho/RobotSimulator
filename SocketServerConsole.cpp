#ifdef PY_CONSOLE
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

static const unsigned short Port = 12001;
static const size_t MaxBufLen = 65536;
static HANDLE thread;
#endif

#include "SocketServerConsole.h"

bool SocketServerConsole::initialized = false;

void SocketServerConsole::Initialize()
{
	assert(!initialized);
	initialized = true;

#ifdef PY_CONSOLE
	thread = CreateThread(NULL, 0, SocketServerThread, NULL, 0, NULL);
#endif // PY_CONSOLE
}

#ifdef PY_CONSOLE
DWORD WINAPI SocketServerConsole::SocketServerThread(LPVOID param)
{
	assert(initialized);
	SOCKET serversoc;
	SOCKET clientsoc;
	SOCKADDR_IN serveraddr;
	SOCKADDR_IN clientaddr;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa); //初始化WS2_32.DLL
	if ((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
	{
		return -1;
	}
	//命名协议，IP，端口
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(Port);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(serversoc, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
	{
		return -1;
	}
	printf("Open Listening\n");
	if (listen(serversoc, 1) != 0)
	{
		return -1;
	}
	int len = sizeof(SOCKADDR_IN);
	while (true)
	{
		if ((clientsoc = accept(serversoc, (SOCKADDR*)&clientaddr, &len)) > 0)
		{
			printf("New Client\n");
			char buf_msg[MaxBufLen];
			int ret_val = 0;
			while (true)
			{
				memset(buf_msg, 0, MaxBufLen);
				ret_val = ::recv(clientsoc, buf_msg, MaxBufLen, 0);
				if (ret_val >= 0)
				{
					printf("From Client: %s\n", buf_msg);
					CE::Console::AddCmdToQueue(buf_msg);
				}
				else
				{
					printf("Failed Receiveing Message From Client\n");
					::closesocket(clientsoc);
					break;
				}
			}
			::shutdown(clientsoc, SD_SEND);
		}
	}
	return 0;
}
#endif // PY_CONSOLE

void SocketServerConsole::Destroy()
{
	assert(initialized);
#ifdef PY_CONSOLE
	TerminateThread(thread, 0);
	WSACleanup();
#endif // PY_CONSOLE
}