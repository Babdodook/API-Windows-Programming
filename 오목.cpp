#include <windows.h>
#include<math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("����");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


#define BESIZE 40
#define SIZE 11
#define WHITE 1
#define BLACK 2

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;

	static int x, y;
	static int flag;
	static int wflag;
	static int bflag;
	static int win;

	static int Map[SIZE][SIZE];
	static int rock[SIZE][SIZE];

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		MoveWindow(hWnd, 0, 0, 600, 480, true); //ȭ�� ũ�� ����

		x = 0, y = 0;
		flag=WHITE;
		win = -1;
		wflag = 1;
		bflag = 1;

		break;
	case WM_TIMER:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//�� �׸��� 11x11
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				hBrush = CreateSolidBrush(RGB(237, 169, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

				Rectangle(hdc, i*BESIZE, j*BESIZE, (i + 1) * BESIZE, (j + 1) * BESIZE);

				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
			
		//�� �׸���
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				if (rock[i][j] == WHITE) {
					Ellipse(hdc, i*BESIZE, j*BESIZE, (i + 1) * BESIZE, (j + 1)*BESIZE);
				}
				if (rock[i][j] == BLACK) {
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

					Ellipse(hdc, i*BESIZE, j*BESIZE, (i + 1) * BESIZE, (j + 1)*BESIZE);

					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}

		TextOut(hdc, 450, 10, TEXT("�鵹 ���� ����"),14);

		//���� ���
		if (win == WHITE) {
			if (MessageBox(hWnd, TEXT("�鵹 ��"), TEXT("���� ���"), MB_RETRYCANCEL) == IDRETRY)
			{
				for (int i = 0; i < SIZE; i++) {
					for (int j = 0; j < SIZE; j++) {
						rock[i][j] = NULL;
					}
				}

				wflag = bflag = 1;
				flag = WHITE;
				win = -1;
			}
			else
				PostQuitMessage(0);
		}
		if (win == BLACK) {
			if (MessageBox(hWnd, TEXT("�浹 ��"), TEXT("���� ���"), MB_RETRYCANCEL) == IDRETRY)
			{
				for (int i = 0; i < SIZE; i++) {
					for (int j = 0; j < SIZE; j++) {
						rock[i][j] = NULL;
					}
				}

				wflag = bflag = 1;
				flag = WHITE;
				win = -1;
			}
			else
				PostQuitMessage(0);
		}
	
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam)/BESIZE;
		y = HIWORD(lParam)/BESIZE;

		if (rock[x][y] != NULL)
			break;

		//�鵹, �浹 ��ǥ ����
		if (x<SIZE && y<SIZE) 
		{
			if (flag == WHITE) {
				rock[x][y] = WHITE;
			}
			if (flag == BLACK)
				rock[x][y] = BLACK;
		}

		/***************���� ����***************/

		wflag = bflag = 1;

		//���� ����
		for (int i = 0; i < SIZE; i++) 
		{
			if (rock[i][y] == rock[i + 1][y])
			{
				if (rock[i][y] == WHITE) 
				{
					wflag++;
					if (wflag == 5) 
					{
						win = WHITE;
					}
				}
				else
					wflag = 1;

				if (rock[i][y] == BLACK) 
				{
					bflag++;
					if (bflag == 5) 
					{
						win = BLACK;
					}
				}
				else
					bflag = 1;
			}
		}

		wflag = bflag = 1;

		//���� ����
		for (int i = 0; i < SIZE; i++) 
		{
			if (rock[x][i] == rock[x][i + 1]) 
			{
				if (rock[x][i] == WHITE) 
				{
					wflag++;
					if (wflag == 5) 
					{
						win = WHITE;
					}
				}
				else
					wflag = 1;

				if (rock[x][i] == BLACK)
				{
					bflag++;
					if (bflag == 5)
					{
						win = BLACK;
					}
				}
				else
					bflag = 1;
			}
		}

		wflag = bflag = 1;

		//�����ʴ밢�� ���� ��
		for (int i = 0; i < SIZE; i++)
		{
			if (rock[(x + i) - 4][(y + i) - 4] == rock[(x + i) - 3][(y + i) - 3])
			{
				if (rock[(x + i) - 4][(y + i) - 4] == WHITE)
				{
					wflag++;
					if (wflag == 5)
						win = WHITE;
				}
				else
					wflag = 1;

				if (rock[(x + i) - 4][(y + i) - 4] == BLACK)
				{
					bflag++;
					if (bflag == 5)
						win = BLACK;
				}
				else
					bflag = 1;
			}
		}

		wflag = bflag = 1;

		//���ʴ밢�� ���� ��
		for (int i = 0; i < SIZE; i++)
		{
			if (rock[(x - 4) + i][(y + 4) - i] == rock[(x - 3) + i][(y + 3) - i]) 
			{
				if (rock[(x - 4) + i][(y + 4) - i] == WHITE)
				{
					wflag++;
					if (wflag == 5)
						win = WHITE;
				}
				else
					wflag = 1;

				if (rock[(x - 4) + i][(y + 4) - i] == BLACK)
				{
					bflag++;
					if (bflag == 5)
						win = BLACK;
				}
				else
					bflag = 1;
			}
		}

		//������ ���� ��
		if(rock[x][y])
		if (flag == WHITE) {
			flag = BLACK;
		}
		else
			flag = WHITE;

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}