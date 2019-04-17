/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicCGuiTemplate.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 17/04/2019
;*
;* Version           : v1.0.0 (build 01 - Stable)
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Using Dialog to set main window BG style.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 17042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include "resource.h"
#include <windows.h>

//DEFINE
#define MAXABOUTBUFFER 512
#define MINABOUTBUFFER 64

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND , UINT , WPARAM , LPARAM );

//DATA
static char szAppName[] = {"Basic GUI - Template v1.0.0"};
static char szOsError[] = {"This program only runs on Windows 95 or abrove"};

int iClientWidth = 640;
int iClientHeight = 480;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {

	//WNDCLASS -> RegisterClass -> CreateWindow -> ShowWindow -> UpdateWindow -> MsgLoop

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//Redraws the entire window if a movement or size adjustment changes.
	wndClass.lpfnWndProc = WndProc;									//A pointer to the window procedure.
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;									//A handle to the instance that contains the window procedure for the class.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			//A handle to the class background brush.
	wndClass.lpszMenuName = (LPCSTR)IDR_MENU;								//The resource name of the class menu, as the name appears in the resource file.
	wndClass.lpszClassName = szAppName;								//The maximum length for lpszClassName is 256.

	if (!RegisterClass(&wndClass)) {								//Value is zero if Register FAIL (!0) = TRUE;
		MessageBox(NULL, szOsError, szAppName, MB_ICONERROR);
		return 0;
	}

	HWND hWnd;
	hWnd = CreateWindow(szAppName, szAppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, iClientWidth, iClientHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);		// nCmdShow
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

static HINSTANCE hInstanceResource;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA

	//CODE
	switch (message) {
	case WM_CREATE:
		hInstanceResource = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			if (!lParam) {
		case ID_PROGRAM_EXIT:
			SendMessage(hwnd, WM_CLOSE, wParam, lParam);
			break;
			}
		case ID_HELP_ABOUT:
			DialogBox(hInstanceResource, (LPCSTR)IDD_DIALOG_ABOUT, hwnd, AboutDlgProc);
			break;
		}
		break;
	case WM_CLOSE:
		switch (MessageBox(hwnd, TEXT("Do you really want to quite?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
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
	static char szAboutDetail[MAXABOUTBUFFER];

	switch (message) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ABOUT_IDDETAIL:
			LoadString(hInstanceResource, IDS_STRING_ABOUTDETAIL, szAboutDetail, MAXABOUTBUFFER);
			MessageBox(hAboutDlgProc, szAboutDetail, TEXT("About - Detail"), MB_OK);
			break;
		case IDCANCEL:
		case ABOUT_IDOK:
			EndDialog(hAboutDlgProc, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}
