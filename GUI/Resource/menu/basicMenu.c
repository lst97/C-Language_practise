/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicMenu.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 14/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Show a Menu
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 14042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include "resource.h"
#include <windowsx.h>
#include <windows.h>

//DEFINE
#define MAXABOUTBUFFER 512
#define MINABOUTBUFFER 64

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[MINABOUTBUFFER];
static char szOsError[MINABOUTBUFFER];
int nClientWidth = 640;
int nClientHeight = 480;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {
	LoadString(hInstance, IDS_STRING_TITLE, szAppName, MINABOUTBUFFER);
	LoadString(hInstance, IDS_STRING_OSERROR, szOsError, MINABOUTBUFFER);
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
		CW_USEDEFAULT, CW_USEDEFAULT, nClientWidth, nClientHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);		// nCmdShow
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA
	static HICON hIcon;
	static HMENU hMenu;
	static int cxIcon, cyIcon, cxClient, cyClient;
	static int menuId;
	static char szBufferAbout[MAXABOUTBUFFER];
	static HDC hdc;
	static HINSTANCE hInstance;
	static PAINTSTRUCT ps;
	static POINT point;

	//CODE
	switch (message) {
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		
		hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));
		hMenu = GetSubMenu(hMenu, 1);
		hwnd = CreateWindow(TEXT("menu"), TEXT("MENU"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT,NULL, hMenu, hInstance, NULL);
		SetMenu(hwnd, hMenu);
		return 0;

	case WM_COMMAND:
		if (!lParam) {
			switch (LOWORD(wParam)) {
			case ID_HELP_ABOUT:
				LoadString(hInstance, IDS_STRING_ABOUT, szBufferAbout, MAXABOUTBUFFER);
				MessageBox(hwnd, szBufferAbout, TEXT("ABOUT"), MB_OK | MB_ICONINFORMATION);
				break;

			case ID_PROGAM_EXIT:
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				break;
			case ID_CHANGEBGCOLOR_BLUE:
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0, 0, 0xff)));
				InvalidateRect(hwnd, NULL, TRUE);
				break;
			case ID_CHANGEBGCOLOR_GREEN:
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0, 0xff, 0)));
				InvalidateRect(hwnd, NULL, TRUE);
				break;
			case ID_CHANGEBGCOLOR_RED:
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(0xff, 0, 0)));
				InvalidateRect(hwnd, NULL, TRUE);
				break;
			case ID_CHANGEBGCOLOR_RANDOM:
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
				InvalidateRect(hwnd, NULL, TRUE);
				break;
			default:
				break;
			}
		}
		return 0;
	case WM_RBUTTONUP:
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		ClientToScreen(hwnd, &point);

		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hwnd, TEXT("Do you really want to quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
