/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicClipBoard.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 21/04/2019
;*
;* Version           : internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Functional Clip Baord
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 21042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include "resource.h"
#include <windows.h>

//DEFINE
#define MAXABOUTBUFFER 512
#define MINBUFFER 64
//////////////////////////////////////////////////////////////////////////

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND , UINT , WPARAM , LPARAM);
//#ifdef
#ifdef UNICODE
#define CF_TCHAR CF_UNICODETEXT
TCHAR szDefaultText[] = TEXT("Default Text - Unicode Version");
TCHAR szCaption[] = TEXT("Clipboard Text Teansfers - Unicode Version");
#else
#define CF_TCHAR CF_TEXT
TCHAR szDefaultText[] = TEXT("Default Text - ANSI Version");
TCHAR szCaption[] = TEXT("Clipboard Text Teansfers - ANSI Version");
#endif
//////////////////////////////////////////////////////////////////////////

//DATA
static HINSTANCE hInstanceResource;
//////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {
	//WNDCLASS -> RegisterClass -> CreateWindow -> ShowWindow -> UpdateWindow -> MsgLoop
	TCHAR szAppName[] = TEXT("Basic GUI - Clip Board");
	TCHAR szOsError[] = TEXT("This program only runs on Windows 2000 or abrove");
	int iClientWidth = 640;
	int iClientHeight = 480;

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//Redraws the entire window if a movement or size adjustment changes.
	wndClass.lpfnWndProc = WndProc;									//A pointer to the window procedure.
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;									//A handle to the instance that contains the window procedure for the class.
	wndClass.hIcon = LoadIcon(hInstance, (LPCWSTR)IDI_ICON);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			//A handle to the class background brush.
	wndClass.lpszMenuName = (LPCWSTR)IDR_MENU;						//The resource name of the class menu, as the name appears in the resource file.
	wndClass.lpszClassName = szAppName;								//The maximum length for lpszClassName is 256.

	if (!RegisterClass(&wndClass)) {								//Value is zero if Register FAIL (!0) = TRUE;
		MessageBox(NULL, szOsError, szAppName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd;
	hwnd = CreateWindow(szAppName, szAppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, iClientWidth, iClientHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);		// nCmdShow
	UpdateWindow(hwnd);

	HACCEL hAccel;
	hAccel = LoadAccelerators(hInstance, (LPCWSTR)IDR_ACCELERATOR);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(hwnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA
	static PTSTR pText;
	BOOL bEnable;
	HGLOBAL hGlobal;
	HDC hdc;
	PTSTR pGlobal;
	PAINTSTRUCT ps;
	RECT rect;

	//CODE
	switch (message) {
	case WM_CREATE:
		hInstanceResource = ((LPCREATESTRUCT)lParam)->hInstance;
		SendMessage(hwnd, WM_COMMAND, ID_EDIT_RESET, 0);
		return 0;
	case WM_INITMENUPOPUP:
		EnableMenuItem((HMENU)wParam, ID_EDIT_PASTE, IsClipboardFormatAvailable(CF_TCHAR) ? MF_ENABLED : MF_GRAYED);	// check if can paste
		bEnable = pText ? MF_ENABLED : MF_GRAYED;
		EnableMenuItem((HMENU)wParam, ID_EDIT_CUT, bEnable);
		EnableMenuItem((HMENU)wParam, ID_EDIT_COPY, bEnable);
		EnableMenuItem((HMENU)wParam, ID_EDIT_CLEAR, bEnable);
		break;

	case WM_COMMAND:
		if (!lParam) {
			switch (LOWORD(wParam)) {
			//PROGAM
			case ID_PROGRAM_EXIT:
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				break;
			////////////////

			//EDIT
			case ID_EDIT_PASTE:
				OpenClipboard(hwnd);
				if (hGlobal = GetClipboardData(CF_TCHAR)) {
					pGlobal = GlobalLock(hGlobal);	// Windows will not nove the memory brock when lock
					if (pText) {
						free(pText);
						pText = NULL;
					}
					pText = malloc(GlobalSize(hGlobal));	// create a memory block to save clipBoard size of data
					lstrcpy(pText, pGlobal);
					InvalidateRect(hwnd, NULL, TRUE);
				}
				CloseClipboard();
				return 0;

			case ID_EDIT_CUT:
			case ID_EDIT_COPY:
				if (!pText) return 0;

				hGlobal = GlobalAlloc(GHND | GMEM_SHARE, (lstrlen(pText) +1) * sizeof(TCHAR));	// create a share memory block
				pGlobal = GlobalLock(hGlobal);	// pointing to satic memory block
				lstrcpy(pGlobal, pText);
				GlobalUnlock(hGlobal);

				OpenClipboard(hwnd);			// modify clip board
				EmptyClipboard();
				SetClipboardData(CF_TCHAR, hGlobal);
				CloseClipboard();

				if (LOWORD(wParam) == ID_EDIT_COPY) return 0;		// for ID_EDIT_CUT
				
			case ID_EDIT_CLEAR:
				if (pText) {
					free(pText);
					pText = NULL;
				}
				InvalidateRect(hwnd, NULL, TRUE);
				return 0;

			case ID_EDIT_RESET:
				if (pText) {
					free(pText);
					pText = NULL;
				}
			default :
				pText = malloc((lstrlen(szDefaultText) + 1) * sizeof(TCHAR));	// default memory size in pText
				lstrcpy(pText, szDefaultText);	// default text
				InvalidateRect(hwnd, NULL, TRUE);
				break;
				return 0;
			////////EDIT////////

			//HELP
			case ID_HELP_ABOUT:
				DialogBox(hInstanceResource, (LPCWSTR)IDD_DIALOG_ABOUT, hwnd, AboutDlgProc);
				break;
			////////////////
				}
			}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		if (pText != NULL) {
			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, pText, -1, &rect, DT_EXPANDTABS | DT_WORDBREAK);
			SetBkMode(hdc, OPAQUE);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hwnd, TEXT("Do you really want to quite?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			if (pText) free(pText);
			PostQuitMessage(0);
			break;
		case IDNO:
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL CALLBACK AboutDlgProc(HWND hAboutDlgProc, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ABOUT_IDDETAIL:;
			static LPWSTR szAboutDetail[MAXABOUTBUFFER];
			LoadString(hInstanceResource, IDS_STRING_ABOUTDETAIL, (LPWSTR)szAboutDetail, MAXABOUTBUFFER);
			MessageBox(hAboutDlgProc, (LPWSTR)szAboutDetail, TEXT("About - Detail"), MB_OK);
			break;
		case IDCANCEL:
		case ABOUT_IDOK:
			EndDialog(hAboutDlgProc, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}
