#pragma comment(lib, "dsound.lib")
#include <Windows.h>
#include<time.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Basic");
HWND G_hWnd;
void DrawBitmap(HDC hdc, int startx, int starty, int endx, int endy, int BitPosx, int BitPosy, HBITMAP hbit, int type);
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

#define BACKGROUND 1
#define OBJECT 2

static int BitmapX, BitmapY;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM
	wParam, LPARAM lParam)
{
	static HDC hdc, BACKDC, MemDC;
	HBITMAP backBitmap;
	static POINT ptMouse;
	RECT rect;
	PAINTSTRUCT pt;

	static int Partition;
	static int x, y;
	static int Width;
	static int Height;
	static int BitPosx[5] = {};
	static int BitPosy[5] = {};
	static int randnum[5] = {};
	static int check = 0;
	static int mi;

	static HBITMAP Bitmap1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

	switch (iMessage)
	{
	case WM_DESTROY:
		DeleteObject(Bitmap1);
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		GetClientRect(hWnd, &rect);
		break;
	case WM_CREATE:
		MoveWindow(hWnd, 0, 0, 800, 600, true);
		srand(time(NULL));
		Partition = 0;
		InvalidateRect(hWnd, NULL, TRUE);
		G_hWnd = hWnd;
		return 0;
	case WM_COMMAND:
		GetClientRect(hWnd, &rect);
		switch (LOWORD(wParam)) {
		case ID_40001:
			Partition = 3;
			Width = rect.right / Partition;
			Height = rect.bottom / Partition;
			break;
		case ID_40002:
			Partition = 4;
			Width = rect.right / Partition;
			Height = rect.bottom / Partition;
			break;
		case ID_40003:
			Partition = 5;
			Width = rect.right / Partition;
			Height = rect.bottom / Partition;
			break;
		}

		memset(randnum, NULL, sizeof(randnum));

		for (int i = 0; i < Partition; i++)
		{
			randnum[i] = rand() % Partition;
			check = 0;
			for (int j = 0; j < Partition; j++)
			{
				if (randnum[i] == randnum[j])
				{
					check++;
				}
			}
			if (check >= 2)
			{
				i--;
			}
		}

		mi = Partition;

		for (int i = 0; i < Partition; i++)
		{
			BitPosx[randnum[i]] = i * Width;
			BitPosy[randnum[i]] = i * Height;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &pt);
		GetClientRect(hWnd, &rect);			//창크기

		BACKDC = CreateCompatibleDC(hdc);//백버퍼(비트맵)와 연결하기위한 DC
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);//백버퍼 만들기
		HBITMAP OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);//백버퍼(비트맵)와 BACKDC연결

		for (int y = 0; y < Partition; y++)
		{
			for (int x = 0; x < Partition; x++)
			{
				DrawBitmap(BACKDC, x*Width, y*Height, Width, Height, BitPosx[x], BitPosy[y], Bitmap1, OBJECT); //백버퍼에 그리기
			}
		}

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, BACKDC, 0, 0, SRCCOPY);//백버퍼를 화면 복사

		SelectObject(BACKDC, OldBackBitmap);//되돌려 주기

		DeleteDC(BACKDC);
		DeleteObject(backBitmap);
		EndPaint(hWnd, &pt);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBitmap(HDC hdc, int startx, int starty, int endx, int endy, int BitPosx, int BitPosy, HBITMAP hbit, int type)
{
	HDC MemoryDC;
	BITMAP Bitmap_;
	HBITMAP OldBitmap;
	MemoryDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemoryDC, hbit);
	GetObject(hbit, sizeof(BITMAP), &Bitmap_);
	BitmapX = Bitmap_.bmWidth;
	BitmapY = Bitmap_.bmHeight;

	//StretchBlt(hdc, x, y, 150, 120, MemoryDC, 0, 0, BitmapX, BitmapY, SRCCOPY);
	//GdiTransparentBlt(hdc, x, y, BSIZE, BSIZE, MemoryDC, 0, 0, BitmapX, BitmapY, RGB(255, 0, 0));
	//BitBlt(hdc, 0, 0, BitmapX, BitmapY, MemoryDC, 0, 0, SRCCOPY);
	StretchBlt(hdc, startx, starty, endx, endy, MemoryDC, BitPosx, BitPosy, endx, endy, SRCCOPY);

	SelectObject(MemoryDC, OldBitmap);
	DeleteDC(MemoryDC);
}