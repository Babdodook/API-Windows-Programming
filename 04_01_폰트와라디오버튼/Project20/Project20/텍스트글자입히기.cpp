#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("이재성_2015032111");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
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

enum { ID_R1 = 101, ID_R2, ID_R3, ID_R4, ID_R5, ID_R6, ID_R7 };
HWND r1, r2, r3, r4, r5, r6, r7;
//int Graph = 0;
COLORREF TextColor = RGB(0, 0, 0);
COLORREF BkColor = RGB(0, 0, 0);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static BOOL start;
	static BOOL trans;

	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	TCHAR str[] = TEXT("테스트");
	HFONT hFont, OldFont;

	switch (iMessage) {
	case WM_CREATE:
		start = FALSE;
		trans = FALSE;

		CreateWindow(TEXT("button"), TEXT("TextColor"), WS_CHILD | WS_VISIBLE |
			BS_GROUPBOX, 5, 5, 120, 110, hWnd, (HMENU)0, g_hInst, NULL);		//그룹 박스
		CreateWindow(TEXT("button"), TEXT("BkColor"), WS_CHILD | WS_VISIBLE |
			BS_GROUPBOX, 145, 5, 120, 140, hWnd, (HMENU)1, g_hInst, NULL);		//그룹 박스
		CreateWindow(TEXT("button"), TEXT("Start"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 15, 60, 35, hWnd, (HMENU)2, g_hInst, NULL);
		r1 = CreateWindow(TEXT("button"), TEXT("Red"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON | WS_GROUP,	//실제 그룹, 그룹박스와는 연관이 없다
			10, 20, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		r2 = CreateWindow(TEXT("button"), TEXT("Green"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON,
			10, 50, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		r3 = CreateWindow(TEXT("button"), TEXT("Blue"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON,
			10, 80, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);
		r4 = CreateWindow(TEXT("button"), TEXT("Red"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON | WS_GROUP,
			150, 20, 100, 30, hWnd, (HMENU)ID_R4, g_hInst, NULL);
		r5 = CreateWindow(TEXT("button"), TEXT("Green"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON,
			150, 50, 100, 30, hWnd, (HMENU)ID_R5, g_hInst, NULL);
		r6 = CreateWindow(TEXT("button"), TEXT("Blue"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON,
			150, 80, 100, 30, hWnd, (HMENU)ID_R6, g_hInst, NULL);
		r7 = CreateWindow(TEXT("button"), TEXT("Transparent"), WS_CHILD | WS_VISIBLE |
			BS_AUTORADIOBUTTON,
			150, 110, 100, 30, hWnd, (HMENU)ID_R7, g_hInst, NULL);
		//CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R1);
		//CheckRadioButton(hWnd, ID_R4, ID_R6, ID_R4);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2:
			start = TRUE;
		case ID_R1:
			TextColor = RGB(255, 0, 0);
			break;
		case ID_R2:
			TextColor = RGB(0, 255, 0);
			break;
		case ID_R3:
			TextColor = RGB(0, 0, 255);
			break;
		case ID_R4:
			BkColor = RGB(255, 0, 0);
			trans = FALSE;
			break;
		case ID_R5:
			BkColor = RGB(0, 255, 0);
			trans = FALSE;
			break;
		case ID_R6:
			BkColor = RGB(0, 0, 255);
			trans = FALSE;
			break;
		case ID_R7:
			trans = TRUE;
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (start)
		{
			SetTextColor(hdc, TextColor);
			SetBkColor(hdc, BkColor);

			if (trans)
				SetBkMode(hdc, TRANSPARENT);

			hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
				VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
			OldFont = (HFONT)SelectObject(hdc, hFont);
			TextOut(hdc, 10, 150, str, lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}