#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg1(HWND hDlg1, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg2(HWND hDlg1, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	hInst = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, MainDlgProc);
	return 0;
}

static int bx;
static int by;

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hDlg1 = NULL;
	HWND hDlg2 = NULL;
	LPNMHDR hdr;
	
	HDC hdc;
	PAINTSTRUCT ps;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	BITMAP bit;

	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADIO1:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, Dlg1);
			ShowWindow(hDlg1, SW_SHOW);
			break;
		case IDC_RADIO2:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hDlg, Dlg2);
			ShowWindow(hDlg2, SW_SHOW);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK Dlg1(HWND hDlg1, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	BITMAP bit;

	switch (iMessage)
	{
	case WM_PAINT:
		GetClientRect(hDlg1, &rect);
		hdc = BeginPaint(hDlg1, &ps);
		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

		GetObject(MyBitmap, sizeof(BITMAP), &bit);

		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, MemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
		EndPaint(hDlg1, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg1, 0);
			break;
		}
		break;
	}
	return 0;
}

BOOL CALLBACK Dlg2(HWND hDlg2, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	BITMAP bit;

	switch (iMessage)
	{
	case WM_PAINT:
		GetClientRect(hDlg2, &rect);
		hdc = BeginPaint(hDlg2, &ps);
		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

		GetObject(MyBitmap, sizeof(BITMAP), &bit);

		StretchBlt(hdc, 0, 0, rect.right, rect.bottom, MemDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
		EndPaint(hDlg2, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hDlg2, 0);
			break;
		}
		break;
	}
	return 0;
}
