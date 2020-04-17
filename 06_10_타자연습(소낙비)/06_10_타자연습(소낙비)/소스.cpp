#include <Windows.h>
#include<time.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Basic");
HWND G_hWnd;
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit);
void DisplayText(HDC hdc, RECT rect);
void Init(RECT rect);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_SHIELD);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

static char str[256];
static int count = 0;
static int x[10];
static int y[10];

char words[10][10] =
{
	"스레드",
	"컴퓨터",
	"마우스",
	"프로그램",
	"알고리즘",
	"자료구조",
	"시플플",
	"사이다",
	"코카콜라",
	"보노보노",
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM
	wParam, LPARAM lParam)
{
	static HDC hdc, BACKDC;
	HBITMAP backBitmap;
	RECT rect;

	static char checkWord[256];
	int len;
	
	static bool startStage = true;
	static bool AllClear = false;

	static int times=0;
	static int speed = 1;

	PAINTSTRUCT pt;
	static HBITMAP Bitmap1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	switch (iMessage)
	{
	case WM_DESTROY:
		DeleteObject(Bitmap1);
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		srand(time(NULL));
		Init(rect);
		SetTimer(hWnd, 1, 50, NULL);
		InvalidateRect(hWnd, NULL, FALSE);
		G_hWnd = hWnd;
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			for (int i = 0; i < 10; i++)
			{
				y[i]+=speed;
			}
			break;
		case 2:
			times++;
			break;
		}
		
		if (times == 5) {
			startStage = true;
			speed = 3;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_CHAR:
		GetClientRect(hWnd, &rect);

		len = strlen(str);
		if (wParam == VK_BACK)
		{
			memmove(str + (len - 1), str + len, sizeof(char));
		}
		if (wParam == VK_RETURN)
		{
			wsprintf(checkWord, str);
			memset(str, 0, sizeof(str));

			for (int i = 0; i < 10; i++)
			{
				if (strcmp(checkWord, words[i]) == 0)
				{
					y[i] = rect.bottom;
					count++;

					if (count >= 5 && times == 0)
					{
						startStage = false;
						SetTimer(hWnd, 2, 1000, NULL);
						GetClientRect(hWnd, &rect);
						Init(rect);
					}
					if (count >= 5 && times > 5)
					{
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 2);
						startStage = false;
						AllClear = true;
					}
				}
			}
		}
		else
		{
			str[len] = (TCHAR)wParam;
			str[len + 1] = 0;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &pt);

		GetClientRect(hWnd, &rect);//창크기

		BACKDC = CreateCompatibleDC(hdc);//백버퍼(비트맵)와 연결하기위한 DC     
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);//백버퍼 만들기
		HBITMAP OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);//백버퍼(비트맵)와 BACKDC연결
		
		DrawBitmap(BACKDC, 0, 0, Bitmap1); //백버퍼에 그리기

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, BACKDC, 0, 0, SRCCOPY);//백버퍼를 화면 복사

		if(startStage)
			DisplayText(hdc, rect);
		
		if (0 < times && times < 3 && !AllClear)
			TextOut(hdc, rect.right / 2 - 80, rect.bottom/2, TEXT("스테이지 1 클리어"), 17);

		if (times >= 3 && !startStage && !AllClear)
			TextOut(hdc, rect.right / 2 - 70, rect.bottom / 2, TEXT("스테이지 2 시작"), 15);

		if(AllClear)
			TextOut(hdc, rect.right / 2 - 30, rect.bottom / 2, TEXT("클리어!"), 7);

		SelectObject(BACKDC, OldBackBitmap);//되돌려 주기

		DeleteDC(BACKDC);
		DeleteObject(backBitmap);
		EndPaint(hWnd, &pt);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit)
{
	HDC MemoryDC;
	int BitmapX, BitmapY;
	BITMAP Bitmap_;
	HBITMAP OldBitmap;
	MemoryDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemoryDC, hbit);
	GetObject(hbit, sizeof(BITMAP), &Bitmap_);
	BitmapX = Bitmap_.bmWidth;
	BitmapY = Bitmap_.bmHeight;
	BitBlt(hdc, x, y, BitmapX, BitmapY, MemoryDC, 0, 0, SRCCOPY);
	SelectObject(MemoryDC, OldBitmap);
	DeleteDC(MemoryDC);
}

void DisplayText(HDC hdc, RECT rect)
{
	Rectangle(hdc, rect.right / 2 - 70, rect.bottom - 27, rect.right / 2 + 70, rect.bottom);
	TextOut(hdc, rect.right / 2 - 60, rect.bottom - 20, str, strlen(str));

	for (int i = 0; i < 10; i++)
	{
		if (count < 5)
			TextOut(hdc, x[i], y[i], words[i], strlen(words[i]));
	}
}

void Init(RECT rect)
{
	count = 0;
	for (int i = 0; i < 10; i++)
	{
		x[i] = (rand() % rect.right - 200) + 250;
		y[i] = -(rand() % 300);
	}
}