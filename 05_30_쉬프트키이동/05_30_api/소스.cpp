#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GetKeyState");

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Mes[128] = TEXT("왼쪽 Shift , 오른쪽 Shift , T:시간끌기");
	static TCHAR str1[128] = TEXT("검사대기"), str2[128] = TEXT("검사대기");
	TCHAR str3[128];
	int i;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			lstrcpy(str1, TEXT("왼쪽 검사 : Shift키가 눌러짐"));
		}
		else {
			lstrcpy(str1, TEXT("왼쪽 검사 : Shift키가 안 눌러짐"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_RBUTTONDOWN:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) { //  반환값 0x0000, 이전에 누른적이 없고, 호출시점에도 누르지 않은 상태
						//  반환값 0x0001, 이전에 누른적이 있고, 호출시점에도 누르지 않은 상태
						//  반환값 0x8000, 이전에 누른적이 없고, 호출시점에는 누른 상태
						//  반환값 0x8001, 이전에 누른적이 있고, 호출시점에도 누른 상태
			lstrcpy(str2, TEXT("오른쪽 검사 : Shift키가 눌러짐"));
		}
		else {
			lstrcpy(str2, TEXT("오른쪽 검사 : Shift키가 안 눌러짐"));
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'T':
			for (i = 0; i < 30; i++) {
				wsprintf(str3, TEXT("카운트 = %d"), i);
				SetWindowText(hWnd, str3);
				Sleep(100);
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		TextOut(hdc, 10, 30, str1, lstrlen(str1));
		TextOut(hdc, 10, 50, str2, lstrlen(str2));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}