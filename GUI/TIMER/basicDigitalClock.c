/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicDigitalClock.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 11/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Basic digital clock
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 11042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>

//DEFINE
#define ID_TIMER 1

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DisplayDigit(HDC, int);
void DisplayTwoDigits(HDC, int, BOOL);
void DisplayColon(HDC);
void DisplayTime(HDC, BOOL, BOOL);


//DATA
static char szAppName[] = "Basic Timer - Digital Clock";
static char szErrorMessage[] = "This program only run on Windows NT!";
static int nClientWidth = 640;
static int nClientHeight = 480;

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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//A handle to the class background brush.
	wndClass.lpszMenuName = NULL;									//The resource name of the class menu, as the name appears in the resource file.
	wndClass.lpszClassName = szAppName;								//The maximum length for lpszClassName is 256.

	if (!RegisterClass(&wndClass)) {								//Value is zero if Register FAIL (!0) = TRUE;
		MessageBox(NULL, szErrorMessage, szAppName, MB_ICONERROR);
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

void DisplayDigit(HDC hDc, int iNumber) {
	static BOOL boolSevenSegmentDisplay[10][7] = {
		1, 1, 1, 0, 1, 1, 1,			// 0
		0, 0, 1, 0, 0, 1, 0,			// 1
		1, 0, 1, 1, 1, 0, 1,			// 2
		1, 0, 1, 1, 0, 1, 1,			// 3
		0, 1, 1, 1, 0, 1, 0,			// 4
		1, 1, 0, 1, 0, 1, 1,			// 5
		1, 1, 0, 1, 1, 1, 1,			// 6
		1, 0, 1, 0, 0, 1, 0,			// 7
		1, 1, 1, 1, 1, 1, 1,			// 8
		1, 1, 1, 1, 0, 1, 1				// 9
	};

	static POINT ptSevenSegmentDisplay[7][6] = {
		7, 6, 11, 2, 31, 2, 35, 6, 31, 10, 11, 10,				// A
		6, 7, 10, 11, 10, 31, 6, 35, 2, 31, 2, 11,				// B
		36, 7, 40, 11, 40, 31, 36, 35, 32, 31, 32, 11,			// C
		7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,			// D
		6, 37, 10, 41, 10, 61, 6, 65, 2, 61, 2, 41,				// E
		36, 37, 40, 41 ,40, 61, 36, 65, 32, 61, 32, 41,			// F
		7, 66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70			// G
	};

	for (unsigned short iSevenSegmentDisplay = 0; iSevenSegmentDisplay < 7; iSevenSegmentDisplay++) {		// 0 = A etc.
		if (boolSevenSegmentDisplay[iNumber][iSevenSegmentDisplay]) {
			Polygon(hDc, ptSevenSegmentDisplay[iSevenSegmentDisplay], 6);
		}
	}
}

void DisplayTwoDigits(HDC hDc, int iNumber, BOOL boolFill) {	// boolFill = TRUE "01" boolFill = FALSE "1" 
	if (!boolFill || (iNumber / 10 != 0)) {						// Second judgement means show the then digite if only single digit > 10 E.G: 1+9 = '10' | 1+8 = '9' not '09'
		DisplayDigit(hDc, iNumber / 10);
	}
	OffsetWindowOrgEx(hDc, -42, 0, NULL);						// Tenth digit
	DisplayDigit(hDc, iNumber % 10);
	OffsetWindowOrgEx(hDc, -42, 0, NULL);						// Single digit
}

void DisplayColon(HDC hDc) {
	POINT ptColon[2][4] = {										// Display ':'
		2, 21, 6, 17, 10, 21, 6, 25,
		2, 51, 6, 47, 10, 51, 6, 55
	};

	Polygon(hDc, ptColon[0], 4);
	Polygon(hDc, ptColon[1], 4);

	OffsetWindowOrgEx(hDc, -12, 0, NULL);
}

void DisplayTime(HDC hDc, BOOL bool24Hour, BOOL boolFill) {
	SYSTEMTIME st;
	GetLocalTime(&st);

	if (bool24Hour) {
		DisplayTwoDigits(hDc, st.wHour, boolFill);
	}
	else {
		DisplayTwoDigits(hDc, (st.wHour %= 12) ? st.wHour : 12, boolFill);
	}
	DisplayColon(hDc);
	DisplayTwoDigits(hDc, st.wMinute, FALSE);					// Min
	DisplayColon(hDc);
	DisplayTwoDigits(hDc, st.wSecond, FALSE);					// Sec
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA
	HDC hDc;
	PAINTSTRUCT ps;
	TCHAR szBuffer[2];
	static BOOL bool24Hour, boolFill;
	static HBRUSH hBrushRed;
	static int cxClient, cyClient;

	//CODE
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, ID_TIMER, 1000, NULL);
		hBrushRed = CreateSolidBrush(RGB(0xff, 0, 0));						// Color Red
	case WM_SETTINGCHANGE:													// In case user change the system setting, program will react immediately
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		bool24Hour = (szBuffer[0] == '1');

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		boolFill = (szBuffer[0] == '0');

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hDc = BeginPaint(hWnd, &ps);
	
		SetMapMode(hDc, MM_ISOTROPIC);									// Keep the clock ratio
		SetWindowExtEx(hDc, 276, 72, NULL);
		SetViewportExtEx(hDc, cxClient, cyClient, NULL);

		SetWindowOrgEx(hDc, 138, 36, NULL);
		SetViewportOrgEx(hDc, cxClient/ 2, cyClient / 2, NULL);
		SelectObject(hDc, GetStockObject(NULL_PEN));
		SelectObject(hDc, hBrushRed);

		DisplayTime(hDc, bool24Hour, boolFill);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Do you really want to quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			KillTimer(hWnd, ID_TIMER);
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
