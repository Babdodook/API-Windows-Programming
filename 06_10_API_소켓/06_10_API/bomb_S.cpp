#include <windows.h>
#include <stdio.h>
#include <math.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASS	WndClass;
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
	hwnd = CreateWindow("Window Class Name",
		"Server Window",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		600,
		350,
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

#define WM_ASYNC WM_USER+2
#define BSIZE 20
#define STEP	5
POINT MyBomb, OthBomb;
WSADATA wsadata;
SOCKADDR_IN c_addr, addr = { 0 };
SOCKET s, cs;
BOOL MyErase, OthErase;
POINT m_bomb, o_bomb;
void ServerStart(HWND hwnd)
{
	RECT rect;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(s, (LPSOCKADDR)&addr, sizeof(addr));
	WSAAsyncSelect(s, hwnd, WM_ASYNC, FD_ACCEPT);
	GetClientRect(hwnd, &rect);
	OthBomb.x = rect.right - BSIZE;
	OthBomb.y = rect.bottom - BSIZE;
	MyBomb.x = rect.left + BSIZE;
	MyBomb.y = rect.bottom - BSIZE;
	listen(s, 5);
	return;
}
void DrawBoard(HDC hdc)
{
	static POINT MyOld, OthOld;
	SetROP2(hdc, R2_XORPEN);
	SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN));
	SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	if (MyErase)
		Ellipse(hdc, MyOld.x - BSIZE, MyOld.y - BSIZE, MyOld.x + BSIZE, MyOld.y + BSIZE);
	else
		MyErase = TRUE;
	Ellipse(hdc, MyBomb.x - BSIZE, MyBomb.y - BSIZE, MyBomb.x + BSIZE, MyBomb.y + BSIZE);
	MyOld = MyBomb;
	SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
	SelectObject(hdc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	if (OthErase)
		Ellipse(hdc, OthOld.x - BSIZE, OthOld.y - BSIZE, OthOld.x + BSIZE, OthOld.y + BSIZE);
	else
		OthErase = TRUE;
	Ellipse(hdc, OthBomb.x - BSIZE, OthBomb.y - BSIZE, OthBomb.x + BSIZE, OthBomb.y + BSIZE);
	OthOld = OthBomb;
}
void Control(HWND hwnd, WPARAM wParam)
{
	HDC hdc;
	RECT rect;
	char msg[20];
	GetClientRect(hwnd, &rect);
	switch (wParam)
	{
	case VK_LEFT:
		MyBomb.x = MyBomb.x - STEP;
		if (MyBomb.x - BSIZE < rect.left)
			MyBomb.x = MyBomb.x + STEP;
		sprintf(msg, "1 %d", MyBomb.x);
		break;
	case VK_RIGHT:
		MyBomb.x = MyBomb.x + STEP;
		if (MyBomb.x + BSIZE > (int)rect.right / 2)
			MyBomb.x = MyBomb.x - STEP;
		sprintf(msg, "1 %d", MyBomb.x);
		break;
	case VK_SPACE:
		SetTimer(hwnd, 1, 50, NULL);
		m_bomb = MyBomb;
		strcpy(msg, "2");
		break;
	}
	hdc = GetDC(hwnd);
	DrawBoard(hdc);
	ReleaseDC(hwnd, hdc);
	if (cs != NULL)
		send(cs, msg, strlen(msg) + 1, NULL);
}
void RePosition(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	//	MyBomb.x = rect.right - BSIZE;
	MyBomb.y = rect.bottom - BSIZE;
	//	OthBomb.x = rect.left + BSIZE;
	OthBomb.y = rect.bottom - BSIZE;
}
#define MOVING	1
#define FIRE	2
void DecodingMessage(HWND hwnd, char *msg)
{
	HDC hdc;
	int msgKind;
	sscanf(msg, "%d", &msgKind);
	switch (msgKind)
	{
	case MOVING:
		sscanf(msg, "%d %d", &msgKind, &OthBomb.x);
		break;
	case FIRE:
		o_bomb = OthBomb;
		SetTimer(hwnd, 2, 50, NULL);
		break;
	}
	hdc = GetDC(hwnd);
	DrawBoard(hdc);
	ReleaseDC(hwnd, hdc);
}
void FlyingBomb(HWND hwnd, WPARAM wParam)
{
	static double m_x = 0, o_x = 0;
	double tmp, radian;
	int x, y;
	static POINT oldm, oldo;
	HDC hdc;
	hdc = GetDC(hwnd);
	switch (wParam)
	{
	case 1:
		m_x = m_x + 10;
		radian = 3.14159*m_x / 180;
		tmp = sin(radian);
		x = (int)(m_bomb.x + m_x * 2.4);
		y = (int)(m_bomb.y - tmp * 200);
		SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, oldm.x - 5, oldm.y - 5, oldm.x + 5, oldm.y + 5);
		SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
		Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
		oldm.x = x; oldm.y = y;
		if (tmp < 0)
		{
			m_x = 0;
			InvalidateRgn(hwnd, NULL, TRUE);
			KillTimer(hwnd, 1);
		}
		break;
	case 2:
		o_x = o_x + 10;
		radian = 3.14159*o_x / 180;
		tmp = sin(radian);
		x = (int)(o_bomb.x - o_x * 2.4);
		y = (int)(o_bomb.y - tmp * 200);
		SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		SelectObject(hdc, (HBRUSH)GetStockObject(WHITE_BRUSH));
		Ellipse(hdc, oldo.x - 5, oldo.y - 5, oldo.x + 5, oldo.y + 5);
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
		Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
		oldo.x = x; oldo.y = y;
		if (tmp < 0)
		{
			o_x = 0;
			InvalidateRgn(hwnd, NULL, TRUE);
			KillTimer(hwnd, 2);
		}
		break;
	}
	ReleaseDC(hwnd, hdc);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int size, msgLen;
	char buffer[100];
	switch (iMsg)
	{
	case WM_CREATE:
		ServerStart(hwnd);
		RePosition(hwnd);
		break;
	case WM_SIZE:
		RePosition(hwnd);
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
			size = sizeof(c_addr);
			cs = accept(s, (LPSOCKADDR)&c_addr, &size);
			WSAAsyncSelect(cs, hwnd, WM_ASYNC, FD_READ);
			break;
		case FD_READ:
			msgLen = recv(cs, buffer, 100, 0);
			buffer[msgLen] = NULL;
			DecodingMessage(hwnd, buffer);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		MyErase = OthErase = FALSE;
		hdc = BeginPaint(hwnd, &ps);
		DrawBoard(hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		Control(hwnd, wParam);
		break;
	case WM_TIMER:
		FlyingBomb(hwnd, wParam);
		break;
	case WM_DESTROY:
		closesocket(s);
		WSACleanup();
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

