#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

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

#define BSIZE 40
#define MAPSIZE 10
#define WHITE 1
#define BLACK 2

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush,oldBrush;

	static int checkw;
	static int checkb;
	static int next;
	static int rock[MAPSIZE][MAPSIZE];
	static int x, y;

	switch (iMessage) {
	case WM_CREATE:
		checkw = 1;
		checkb = 1;
		next = WHITE;
		memset(rock, 0, sizeof(rock));

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < MAPSIZE; i++)
		{
			for (int j = 0; j < MAPSIZE; j++)
			{
				hBrush = (HBRUSH)CreateSolidBrush(RGB(153,138,0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, i*BSIZE, j*BSIZE, (i + 1)*BSIZE, (j + 1)*BSIZE);
			}
		}

		for (int i = 0; i < MAPSIZE; i++)
		{
			for (int j = 0; j < MAPSIZE; j++)
			{
				if (rock[i][j] == WHITE)
				{
					hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i*BSIZE, j*BSIZE, (i + 1)*BSIZE, (j + 1)*BSIZE);
				}
				if (rock[i][j] == BLACK)
				{
					hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, i*BSIZE, j*BSIZE, (i + 1)*BSIZE, (j + 1)*BSIZE);
				}
			}
		}

		if (checkw == 5)
		{
			TextOut(hdc, 500, 50, TEXT("백돌승리"),8);
		}
		if (checkb == 5)
		{
			TextOut(hdc, 470, 10, TEXT("흑돌승리"), 8);
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam) / BSIZE;
		y = HIWORD(lParam) / BSIZE;

		//이미 놓아진 돌 중복클릭 체크
		if (rock[x][y] != NULL)
		{
			return 0;
		}


		//바둑판 내부 클릭시 돌을 놓음
		if (x < MAPSIZE && y < MAPSIZE)
		{
			if (next == WHITE) {
				rock[x][y] = WHITE;
			}
			if (next == BLACK)
			{
				rock[x][y] = BLACK;
			}
		}

		//가로 조건식
		for (int i = 0; i < MAPSIZE; i++)
		{
			if (rock[i][y] == rock[i + 1][y])
			{
				if (rock[i][y] == WHITE)
				{
					checkw++;
					if (checkw == 5)
					{
						return 0;
					}
				}

				if (rock[i][y] == BLACK)
				{
					checkb++;
					if (checkb == 5)
					{
						return 0;
					}
				}
			}
			else
			{
				checkw = 1;
				checkb = 1;
			}
		}

		//다음 차례에 놓을 돌
		if (next == WHITE)
		{
			next = BLACK;
		}
		else 
		{
			next = WHITE;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
