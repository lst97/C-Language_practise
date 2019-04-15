/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicDialogBox.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 15/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Using Dialog to set main window BG color.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 15042019    lst97       1      First release
;*
;* Known Issue       :
;* Random color not function correctly, fix later.
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
BOOL CALLBACK SetCircleDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM,LPARAM);
void PaintWindows(HWND, int, int);
void PaintTheBlock(HWND, int, int);


//DATA
static char szAppName[MINABOUTBUFFER];
static char szOsError[MINABOUTBUFFER];
int iClientWidth = 640;
int iClientHeight = 480;

int iCurrentColor = IDC_SETCIRCLE_RADIO_RED;
int iCurrentFigure = IDC_SETCIRCLE_RADIO_RECTANGLE;

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
		CW_USEDEFAULT, CW_USEDEFAULT, iClientWidth, iClientHeight, NULL, NULL, hInstance, NULL);

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
	static HINSTANCE hInstance;
	PAINTSTRUCT ps;

	//CODE
	switch (message) {
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_HELP_ABOUT:
			DialogBox(hInstance, (LPCSTR)IDD_DIALOG_ABOUT, hwnd, AboutDlgProc);
			break;
		case ID_SETTING_SETCIRCLE:
			DialogBox(hInstance, (LPCSTR)IDD_DIALOG_SETCIRCLE, hwnd, SetCircleDlgProc);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		return 0;
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);

		PaintWindows(hwnd, iCurrentColor, iCurrentFigure);
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

BOOL CALLBACK SetCircleDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hCtrlBlock;
	static int iColor, iFigure;

	switch (message) {
	case WM_INITDIALOG:
		iColor = iCurrentColor;
		iFigure = iCurrentFigure;

		CheckRadioButton(hDlg, IDC_SETCIRCLE_RADIO_RED, IDC_SETCIRCLE_RADIO_RANDOM, iColor);
		CheckRadioButton(hDlg, IDC_SETCIRCLE_RADIO_RECTANGLE, IDC_SETCIRCLE_RADIO_ELIPSE, iFigure);

		hCtrlBlock = GetDlgItem(hDlg, IDC_SETCIRCLE_DEMO);

		SetFocus(GetDlgItem(hDlg, iColor));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case SETCIRCLE_IDCANCEL:
		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;

		case SETCIRCLE_IDOK:
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDC_SETCIRCLE_RADIO_RED:
		case IDC_SETCIRCLE_RADIO_GREEN:
		case IDC_SETCIRCLE_RADIO_BLUE:
		case IDC_SETCIRCLE_RADIO_RANDOM:
			iColor = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_SETCIRCLE_RADIO_RED, IDC_SETCIRCLE_RADIO_RANDOM, LOWORD(wParam));
			PaintTheBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;

		case IDC_SETCIRCLE_RADIO_RECTANGLE:
		case IDC_SETCIRCLE_RADIO_ELIPSE:
			iFigure = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_SETCIRCLE_RADIO_RECTANGLE, IDC_SETCIRCLE_RADIO_ELIPSE, LOWORD(wParam));
			PaintTheBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;
		}
		break;
	case WM_PAINT:
		PaintTheBlock(hCtrlBlock, iColor, iFigure);
		break;
	}
	return FALSE;
}


BOOL CALLBACK AboutDlgProc(HWND hAboutDlgProc, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:				// TRUE = SetFocus.
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ABOUT_IDOK:
		case IDCANCEL:				// For ESC key
			EndDialog(hAboutDlgProc, 0);
			/*SendMessage(GetParent(hAboutDlgProc), WM_CLOSE, wParam, lParam);		// You can sendMessage to WndProc.*/
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void PaintWindows(HWND hwnd, int iColor, int iFigure) {
	static COLORREF crColor[4] = { RGB(0xFF, 0, 0), RGB(0, 0xFF, 0), RGB(0, 0, 0xFF) , RGB(0, 0, 0) };
	crColor[3] = RGB(rand()%256, rand() % 256, rand() % 256);
	HBRUSH hBrush;
	HDC hdc;
	RECT rect;

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	hBrush = CreateSolidBrush(crColor[iColor - IDC_SETCIRCLE_RADIO_RED]);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);

	if (iFigure == IDC_SETCIRCLE_RADIO_RECTANGLE) {
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	else {
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintTheBlock(HWND hCtrl, int iColor, int iFigure) {
	InvalidateRect(hCtrl, NULL, TRUE);
	UpdateWindow(hCtrl);
	PaintWindows(hCtrl, iColor, iFigure);
}
