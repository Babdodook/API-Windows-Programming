#include <windows.h>
#include <stdio.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	WSADATA wsadata;
	SOCKET s;
	char message[200];
	SOCKADDR_IN addr = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(s, (LPSOCKADDR)&addr, sizeof(addr));
	if (listen(s, 5) == -1)
		return 0;
	do
	{
		SOCKADDR_IN c_addr;
		char buffer[100];
		int msgLen;
		int size = sizeof(c_addr);
		SOCKET cs = accept(s, (LPSOCKADDR)&c_addr, &size);
		msgLen = recv(cs, buffer, 100, 0);
		buffer[msgLen] = NULL;
		sprintf(message, "클라이언트 메시지: %s, 서버를 종료하시겠습니까?",
			buffer);
	} while (MessageBox(NULL, message, "Server 메시지", MB_YESNO) == IDNO);
	closesocket(s);
	WSACleanup();
	return 1;
}