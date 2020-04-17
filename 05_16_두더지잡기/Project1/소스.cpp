#pragma comment(lib, "dsound.lib")
#include <Windows.h>
#include<time.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Basic");
HWND G_hWnd;
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit, int type);
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

#include <math.h>

#define BACKGROUND 1
#define OBJECT 2

#define BSIZE 100

static int Bitx, Bity;			//��Ʈ����ǥ

double LengthPts(int x1, int y1, int x2, int y2)
{
	return(sqrt((float)(x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LengthPts(x, y, mx, my) < BSIZE)
		return TRUE;
	else
		return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM
	wParam, LPARAM lParam)
{
	static HDC hdc, BACKDC;
	HBITMAP backBitmap;
	static POINT ptMouse;
	RECT rect;
	PAINTSTRUCT pt;

	TCHAR str[128] = {};
	static int mx, my;			//���콺 ��ǥ
	static bool draw;			//Ÿ�̸�üũ
	static bool select;
	static int x, y;
	static int score;

	static HBITMAP Bitmap1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1)),
		Bitmap2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));

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
		select = false;
		mx = my = 0;
		Bitx = Bity = 0;
		score = 0;
		draw = true;
		srand(time(NULL));
		SetTimer(hWnd, 1, 500, NULL);
		InvalidateRect(hWnd, NULL, TRUE);
		G_hWnd = hWnd;
		return 0;
	case WM_TIMER:
		draw = !draw;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (InCircle(x, y, mx, my))
		{
			score++;
		}
		//InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &pt);
		GetClientRect(hWnd, &rect);			//âũ��

		x = rand() % (rect.right - BSIZE) + BSIZE;
		y = rand() % (rect.bottom - BSIZE);

		BACKDC = CreateCompatibleDC(hdc);//�����(��Ʈ��)�� �����ϱ����� DC
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);//����� �����
		HBITMAP OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);//�����(��Ʈ��)�� BACKDC����

		DrawBitmap(BACKDC, rect.right, rect.bottom, Bitmap2, BACKGROUND);

		if (draw)
			DrawBitmap(BACKDC, x, y, Bitmap1, OBJECT); //����ۿ� �׸���

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, BACKDC, 0, 0, SRCCOPY);//����۸� ȭ�� ����

		SelectObject(BACKDC, OldBackBitmap);//�ǵ��� �ֱ�

		DeleteDC(BACKDC);
		DeleteObject(backBitmap);

		wsprintf(str, TEXT("���� ������ %d�� �Դϴ�."), score);
		TextOut(hdc, 0, 50, str, lstrlen(str));

		EndPaint(hWnd, &pt);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit, int type)
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
	switch (type)
	{
	case BACKGROUND:
		GdiTransparentBlt(hdc, 0, 0, x, y, MemoryDC, 0, 0, BitmapX, BitmapY, RGB(255, 0, 0));
		break;
	case OBJECT:
		//StretchBlt(hdc, x, y, 150, 120, MemoryDC, 0, 0, BitmapX, BitmapY, SRCCOPY);
		GdiTransparentBlt(hdc, x, y, BSIZE, BSIZE, MemoryDC, 0, 0, BitmapX, BitmapY, RGB(255, 0, 0));
		Bitx = x;
		Bity = y;
		break;
	}
	SelectObject(MemoryDC, OldBitmap);
	DeleteDC(MemoryDC);
}