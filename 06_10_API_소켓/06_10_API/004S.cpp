#include <windows.h>
#include <stdio.h>
#define WM_ASYNC WM_USER+2  // 기존 위도우 메시지와의 충돌을 방지하기 위해 사용자 정의 윈도우 메시지 사용

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	 hwnd;
	MSG 	 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow(
		"Window Class Name",
		"Server Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static WSADATA wsadata;
	static SOCKET s, cs;
	static char msg[200];
	static SOCKADDR_IN addr = { 0 }, c_addr;
	int size, msgLen;
	char buffer[100];

	switch (iMsg)
	{
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &wsadata);
		s = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		bind(s, (LPSOCKADDR)&addr, sizeof(addr));
		WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);
		if (listen(s, 5) == -1) return 0;
		break;
	case WM_ASYNC: // 특정 네트워크의 이벤트 발생 시 발생하는 윈도우 메시지로 사용
		switch (lParam)
		{
		case FD_ACCEPT:  // 접속을 요구하는 신호가 오면 발생, FD_WRITE : 읽을 데이터가 사용 가능하면 발생, FD_CONNECT : 접속이 완료되면 발생
			size = sizeof(c_addr);    //  클라이언트가 접속을 시도하면 AM_ASYNC메시지가 발생하고 lParam의 내용은 FD_ACCEPT가 됨.
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);  //클라이언트와 통신하기 위한 소켓,cs를 만들어 통신을 전담
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);
			break;
		case FD_READ:  // 읽을 준비가 되면 발생, 클라이언트가 데이터를 보내면 AM_ASYNS메시지가 발생하고 lParam의 내용은 FD_READ가 됨. 소켓cs에서 데이터를 받고 버퍼에 저장.
			msgLen = recv(cs, buffer, 100, 0);
			buffer[msgLen] = NULL;
			strcpy(msg, buffer);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
				   break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 0, 0, msg, (int)strlen(msg));
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
