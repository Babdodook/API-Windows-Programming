#include <windows.h>
#include <commctrl.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg1(HWND hDlg1, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND hDlgMain;
HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, MainDlgProc);
	return 0;
}

static char PrintName[50][50] = {};
static int Pindex = 0;
HWND Dlg1hwnd;

void MakeColumn(HWND hDlg)
{
	char *name[2] = { (char*)"이름", (char*)"전화번호" };
	LVCOLUMN lvCol = { 0, };
	HWND hList;
	int i;
	hList = GetDlgItem(hDlg, IDC_LIST1);
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	for (i = 0; i < 2; i++)
	{
		lvCol.cx = 90;
		lvCol.pszText = name[i];
		lvCol.iSubItem = i;
		SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&lvCol);
	}
}

void InsertData(HWND hDlg)
{
	int count;
	LVITEM item;
	HWND hList;
	char name[20], phone[20];

	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	GetDlgItemText(hDlg, IDC_EDIT1, PrintName[Pindex], 20);
	SetDlgItemText(hDlg, IDC_EDIT1, "");
	if (strcmp(name, "") == 0) return;
	GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT2, "");

	hList = GetDlgItem(hDlg, IDC_LIST1);
	count = ListView_GetItemCount(hList);
	item.mask = LVIF_TEXT;
	item.iItem = count;
	item.iSubItem = 0;
	item.pszText = name;
	ListView_InsertItem(hList, &item);
	ListView_SetItemText(hList, count, 1, phone);
}

int SelectItem(HWND hDlg, LPARAM lParam)
{
	LPNMLISTVIEW nlv;
	HWND hList;
	char name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST1);
	nlv = (LPNMLISTVIEW)lParam;
	ListView_GetItemText(hList, nlv->iItem, 0, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT1, name);
	ListView_GetItemText(hList, nlv->iItem, 1, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT2, phone);
	return(nlv->iItem);
}

void ModifyItem(HWND hDlg, int selection)
{
	static HWND hList;
	char name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST1);
	GetDlgItemText(hDlg, IDC_EDIT1, name, 20);
	GetDlgItemText(hDlg, IDC_EDIT2, phone, 20);
	if (strcmp(name, "") == 0) return;
	ListView_SetItemText(hList, selection, 0, name);
	ListView_SetItemText(hList, selection, 1, phone);
	SetDlgItemText(hDlg, IDC_EDIT1, "");
	SetDlgItemText(hDlg, IDC_EDIT2, "");
	return;
}

void DeleteItem(HWND hDlg, int selection)
{
	static HWND hList;
	hList = GetDlgItem(hDlg, IDC_LIST1);
	GetDlgItemText(hDlg, IDC_EDIT1, PrintName[Pindex], 20);
	ListView_DeleteItem(hList, selection);
	SetDlgItemText(hDlg, IDC_EDIT1, "");
	SetDlgItemText(hDlg, IDC_EDIT2, "");
	return;
}

#define UNSELECTED -1
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hDlg1 = NULL;
	LPNMHDR hdr;
	HWND hList;
	static int selection;

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		selection = UNSELECTED;
		MakeColumn(hDlg);
		return TRUE;
	case WM_NOTIFY:
		hdr = (LPNMHDR)lParam;
		hList = GetDlgItem(hDlg, IDC_LIST1);
		if (hdr->hwndFrom == hList && hdr->code == LVN_ITEMCHANGING)
			selection = SelectItem(hDlg, lParam);
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, Dlg1);
			ShowWindow(hDlg1, SW_SHOW);
			break;
		case IDOK2:
			InsertData(hDlg);
			strcat(PrintName[Pindex], "가 가입하였습니다");
			Pindex++;
			break;
		case IDOK3:
			if (selection == UNSELECTED) break;
			DeleteItem(hDlg, selection);
			selection = UNSELECTED;

			strcat(PrintName[Pindex], "가 탈퇴하였습니다");
			Pindex++;
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

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = GetDC(hDlg1);
		for (int i = 0; i < Pindex; i++)
		{
			TextOut(hdc, 30, 20 * i, PrintName[i], strlen(PrintName[i]));
		}
		ReleaseDC(hDlg1, hdc);
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
