#include <windows.h>
#include <stdio.h>
#define WM_ASYNC WM_USER+2  // ���� ������ �޽������� �浹�� �����ϱ� ���� ����� ���� ������ �޽��� ���

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
	case WM_ASYNC: // Ư�� ��Ʈ��ũ�� �̺�Ʈ �߻� �� �߻��ϴ� ������ �޽����� ���
		switch (lParam)
		{
		case FD_ACCEPT:  // ������ �䱸�ϴ� ��ȣ�� ���� �߻�, FD_WRITE : ���� �����Ͱ� ��� �����ϸ� �߻�, FD_CONNECT : ������ �Ϸ�Ǹ� �߻�
			size = sizeof(c_addr);    //  Ŭ���̾�Ʈ�� ������ �õ��ϸ� AM_ASYNC�޽����� �߻��ϰ� lParam�� ������ FD_ACCEPT�� ��.
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);  //Ŭ���̾�Ʈ�� ����ϱ� ���� ����,cs�� ����� ����� ����
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);
			break;
		case FD_READ:  // ���� �غ� �Ǹ� �߻�, Ŭ���̾�Ʈ�� �����͸� ������ AM_ASYNS�޽����� �߻��ϰ� lParam�� ������ FD_READ�� ��. ����cs���� �����͸� �ް� ���ۿ� ����.
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
