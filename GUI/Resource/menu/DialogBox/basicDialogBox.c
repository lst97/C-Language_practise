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
;* Purpose           : Using Dialog to set main window BG style.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 15042019    lst97       1      First release
;* 16042019    lst97       2      Fix random problem
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
BOOL CALLBACK ChangeBgStyleDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM,LPARAM);
void PaintWindow(HWND, int, int, BOOL);
void PaintDemoArea(HWND, int, int, BOOL);


//DATA
static char szAppName[MINABOUTBUFFER];
static char szOsError[MINABOUTBUFFER];
static char szAboutDeatil[MAXABOUTBUFFER];
int iClientWidth = 640;
int iClientHeight = 480;

int iCurrentColor = IDC_CHANGEBGSTYLE_RADIO_RED;
int iCurrentFigure = IDC_CHANGEBGSTYLE_RADIO_RECTANGLE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {
	LoadString(hInstance, IDS_STRING_TITLE, szAppName, MINABOUTBUFFER);
	LoadString(hInstance, IDS_STRING_OSERROR, szOsError, MINABOUTBUFFER);
	LoadString(hInstance, IDS_STRING_ABOUT, szAboutDeatil, MAXABOUTBUFFER);
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
			DialogBox(hInstance, (LPCSTR)IDD_DIALOG_ABOUT, hwnd, AboutDlgProc);		// Show about Dialogbox
			break;
		case ID_SETTING_CHANGEBGSTYLE:
			DialogBox(hInstance, (LPCSTR)IDD_DIALOG_CHANGEBGSTYLE, hwnd, ChangeBgStyleDlgProc);		// Show Style Dialogbox
			InvalidateRect(hwnd, NULL, TRUE);			// set to invalid so dialogbox can change the style.
			break;
		case ID_PROGAM_EXIT:
			SendMessage(hwnd, WM_CLOSE, wParam, lParam);
			break;
		}
		return 0;
	case WM_PAINT:
		BeginPaint(hwnd, &ps);
		PaintWindow(hwnd, iCurrentColor, iCurrentFigure, FALSE);
		EndPaint(hwnd, &ps);

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

BOOL CALLBACK ChangeBgStyleDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hDlgPaintArea;			
	static int iColor, iFigure;				// iColor save Group 1 radio id, iFigure save Group 2 radio id.
	BOOL bRadomSwitch = FALSE;

	switch (message) {
	case WM_INITDIALOG:
		iColor = iCurrentColor;				// Default is 1006 (IDC_CHANGEBGSTYLE_RADIO_RED)
		iFigure = iCurrentFigure;

		CheckRadioButton(hDlg, IDC_CHANGEBGSTYLE_RADIO_RED, IDC_CHANGEBGSTYLE_RADIO_RANDOM, iColor);
		CheckRadioButton(hDlg, IDC_CHANGEBGSTYLE_RADIO_RECTANGLE, IDC_CHANGEBGSTYLE_RADIO_ELIPSE, iFigure);

		hDlgPaintArea = GetDlgItem(hDlg, IDC_CHANGEBGSTYLE_DEMO);	// Get handle of the painting area.

		SetFocus(GetDlgItem(hDlg, iColor));	// Set radio focus to red
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {			// LOWORD(wParam) storage the id of which radioBtn was clicked.
		case CHANGEBGSTYLE_IDCANCEL:
		case IDCANCEL:
			EndDialog(hDlg, FALSE);			// TRUE = The value to be returned to the application from the function that created the dialog box.
			return TRUE;

		case CHANGEBGSTYLE_IDOK:
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hDlg, TRUE);
			return TRUE;
		
		case IDC_CHANGEBGSTYLE_RADIO_RANDOM:
			bRadomSwitch = TRUE;
		case IDC_CHANGEBGSTYLE_RADIO_RED:
		case IDC_CHANGEBGSTYLE_RADIO_GREEN:
		case IDC_CHANGEBGSTYLE_RADIO_BLUE:
			iColor = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_CHANGEBGSTYLE_RADIO_RED, IDC_CHANGEBGSTYLE_RADIO_RANDOM, LOWORD(wParam));
			PaintDemoArea(hDlgPaintArea, iColor, iFigure, bRadomSwitch);
			return TRUE;

		case IDC_CHANGEBGSTYLE_RADIO_RECTANGLE:
		case IDC_CHANGEBGSTYLE_RADIO_ELIPSE:
			iFigure = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_CHANGEBGSTYLE_RADIO_RECTANGLE, IDC_CHANGEBGSTYLE_RADIO_ELIPSE, LOWORD(wParam));
			PaintDemoArea(hDlgPaintArea, iColor, iFigure, FALSE);
			return TRUE;
		}
		break;
	case WM_PAINT:
		PaintDemoArea(hDlgPaintArea, iColor, iFigure, FALSE);
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutDlgProc(HWND hAboutDlgProc, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:				
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ABOUT_IDDETAIL:
			MessageBox(hAboutDlgProc, szAboutDeatil, TEXT("About - Detail"), MB_OK);
			break;
		case ABOUT_IDOK:
		case IDCANCEL:				// For ESC key
			EndDialog(hAboutDlgProc, FALSE);
			/*SendMessage(GetParent(hAboutDlgProc), WM_CLOSE, wParam, lParam);		// You can sendMessage to WndProc.*/
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void PaintWindow(HWND hwnd, int iColor, int iFigure, BOOL bRandomSwitch) {
	static COLORREF crColor[4];
	HBRUSH hBrush;
	HDC hdc;
	RECT rect;

	hdc = GetDC(hwnd);
	unsigned short arrayCounter = 0;
	for (unsigned int loopCounter = 0xFF; loopCounter <= 0xFFFFFF; loopCounter <<= 8) { //S Set color table (Red Green Blue)
		crColor[arrayCounter] = loopCounter;
		arrayCounter++;
	}
	if (bRandomSwitch) crColor[3] = RGB(rand() % 256, rand() % 256, rand() % 256);		// Change color online IDC_CHANGEGBSTYLE_RADIO_RANDOM click

	GetClientRect(hwnd, &rect);
	hBrush = CreateSolidBrush(crColor[iColor - IDC_CHANGEBGSTYLE_RADIO_RED]);			// IDC_CHANGEBGSTYLE_RADIO_RED is a color id base if Green selected, number will be 1007 - 1006 = 1, it needs to create correct ID number order.
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);

	if (iFigure == IDC_CHANGEBGSTYLE_RADIO_RECTANGLE) {									// Draw on the WndProc.
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}else Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintDemoArea(HWND hCtrl, int iColor, int iFigure, BOOL bRandomClick) {
	InvalidateRect(hCtrl, NULL, TRUE);
	UpdateWindow(hCtrl);
	PaintWindow(hCtrl, iColor, iFigure, bRandomClick);
}
