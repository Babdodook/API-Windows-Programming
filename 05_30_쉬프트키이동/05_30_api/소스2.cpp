#include <windows.h>
#include"resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("MoveSharp");


void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit,int type);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, BACKDC, MemDC;
	HBITMAP backBitmap;
	RECT rect;
	PAINTSTRUCT ps;
	TCHAR Mes[128] = TEXT("���� Shift , ������ Shift , T:�ð�����");
	static TCHAR str1[128] = TEXT("�˻���"), str2[128] = TEXT("�˻���");
	TCHAR str3[128];
	int i;

	static HBITMAP Bitmap1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	static POINT mousePt;
	static int x=100;
	static int y=100;
	static bool Drag = false;
	static RECT ImgRect;

	static bool isShift = false;

	switch (iMessage) {
	case WM_CREATE:
		SetRect(&ImgRect, 100, 100, 200, 200);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		mousePt.x = LOWORD(lParam);
		mousePt.y = HIWORD(lParam);

		if (PtInRect(&ImgRect, mousePt))
		{
			Drag = true;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_MOUSEMOVE:
		mousePt.x = LOWORD(lParam);
		mousePt.y = HIWORD(lParam);

		if (GetKeyState(VK_SHIFT) & 0x8000) {
			isShift = true;
		}
		else {
			isShift = false;
		}

		if (Drag)
		{
			x = mousePt.x;
			if(!isShift)
				y = mousePt.y;
			InvalidateRect(hWnd, NULL, FALSE);
		}

		
		return 0;
	case WM_LBUTTONUP:
		Drag = false;
		return 0;
	case WM_RBUTTONDOWN:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) { //  ��ȯ�� 0x0000, ������ �������� ����, ȣ��������� ������ ���� ����
						//  ��ȯ�� 0x0001, ������ �������� �ְ�, ȣ��������� ������ ���� ����
						//  ��ȯ�� 0x8000, ������ �������� ����, ȣ��������� ���� ����
						//  ��ȯ�� 0x8001, ������ �������� �ְ�, ȣ��������� ���� ����
			lstrcpy(str2, TEXT("������ �˻� : ShiftŰ�� ������"));
		}
		else {
			lstrcpy(str2, TEXT("������ �˻� : ShiftŰ�� �� ������"));
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		
		BACKDC = CreateCompatibleDC(hdc);//�����(��Ʈ��)�� �����ϱ����� DC
		backBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);//����� �����
		HBITMAP OldBackBitmap = (HBITMAP)SelectObject(BACKDC, backBitmap);//�����(��Ʈ��)�� BACKDC����

		DrawBitmap(BACKDC, rect.right, rect.bottom, Bitmap1, BACKGROUND);
		DrawBitmap(BACKDC, x, y, Bitmap1, OBJECT);
		SetRect(&ImgRect, x-100, y-100, x+100, y+100);

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, BACKDC, 0, 0, SRCCOPY);//����۸� ȭ�� ����

		SelectObject(BACKDC, OldBackBitmap);//�ǵ��� �ֱ�

		DeleteDC(BACKDC);
		DeleteObject(backBitmap);

		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void DrawBitmap(HDC hdc, int x, int y,HBITMAP hbit, int type)
{
	HDC MemoryDC;
	BITMAP Bitmap_;
	HBITMAP OldBitmap;
	MemoryDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemoryDC, hbit);
	GetObject(hbit, sizeof(BITMAP), &Bitmap_);
	Bitmap_.bmWidth;
	Bitmap_.bmHeight;

	switch (type)
	{
	case 1:
		BitBlt(hdc, 0, 0, x, y, MemoryDC, 0, 0, WHITENESS);
		break;
	case 2:
		StretchBlt(hdc, x-100, y-100, 200, 200, MemoryDC, 0, 0, Bitmap_.bmWidth, Bitmap_.bmHeight, SRCCOPY);
		break;
	}

	SelectObject(MemoryDC, OldBitmap);
	DeleteDC(MemoryDC);
}