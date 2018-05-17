#pragma once

#include "../Engine/CoolEngine.h"

class SocketServerConsole
{
private:
	SocketServerConsole() = delete;
	~SocketServerConsole() = delete;
	SocketServerConsole(const SocketServerConsole&) = delete;
	SocketServerConsole& operator=(const SocketServerConsole&) = delete;
	void *operator new(size_t) = delete;
	void operator delete(void* ptr) = delete;
	static bool initialized;
#ifdef PY_CONSOLE
	static DWORD WINAPI SocketServerThread(LPVOID param);
#endif
public:
	static void Initialize();
	static void Destroy();
#ifdef PY_CONSOLE
	static void SendMsgToClient(SOCKET clientSocket, const char *data, unsigned size);
#endif
};