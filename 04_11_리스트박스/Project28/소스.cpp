#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg3(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	 hwnd;
	MSG 	 msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow("Window Class Name",
		"Window Title Name",
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
	switch (iMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_40001:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Dlg3);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK Dlg3(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HWND hList;
	static int selection;
	static int isMale = false;
	static int isFemale = false;
	char str[200] = "";
	char name[20];
	char hobby[20];
	char sex[20] = "";
	char male[20] = "남자";
	char female[20] = "여자";

	switch (iMsg)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST1);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:	//추가
			GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
			GetDlgItemText(hDlg, IDC_EDIT2, hobby, 20);

			if (isMale)
			{
				strcat(sex, male);
			}
			else if (isFemale)
			{
				strcat(sex, female);
			}

			wsprintf(str, "이름: %s, 취미: %s, 성별: %s", name, hobby, sex);
			if (strcmp(name, "") && strcmp(hobby, ""));
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)str);
			return 0;
		case IDOK2:	//삭제
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			return 0;
		case IDC_LIST1:	//리스트박스
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		case IDCANCEL:	//닫기
			EndDialog(hDlg, 0);
			return 0;
		case IDC_RADIO1:
			isMale = true;
			isFemale = false;
			break;
		case IDC_RADIO2:
			isMale = false;
			isFemale = true;
			break;
		}
		break;
	}
	return 0;
}
