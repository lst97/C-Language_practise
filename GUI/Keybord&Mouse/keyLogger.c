/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : keyLogger.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 11/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : To log all the event of the keybord
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 11032019    lst97       1      First release
;*
;* Known Issue       :
;* Only the layout, to be continue NOT FUNCTIONAL
;*
;|**********************************************************************;
*/

#include <windows.h>
#include "strSafe.h"
//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[] = "Basic Keyboard&Mouse - Keylogger";
static char szErrorMessage[] = "This program only run on Windows NT!";
static int nClientWidth = 640;
static int nClientHeight = 480;

static char szTitleMessage[] = "Message";
static char szTitleKey[] = "Key";
static char szTitleChar[] = "Char";
static char szTitleRepeat[] = "Repeat";
static char szTitleScan[] = "Scan";
static char szTitleExt[] = "Ext";
static char szTitleAlt[] = "ALT";
static char szTitlePrev[] = "Prev";
static char szTitleTran[] = "Tran";

static unsigned int strLength;
static char szMsgBuffer[16];
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	RECT rectScroll;
	TEXTMETRICA tm;
	PMSG pmsg;
	//DATA

	//CODE
	switch(message) {
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:

		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		GetClientRect(hWnd, &rectScroll);
		GetTextMetrics(hdc, &tm);
		rectScroll.top += (tm.tmHeight + 5);
		ScrollWindow(hWnd, 0, -rect.bottom / 25, &rectScroll, &rectScroll);
		ReleaseDC(hWnd, hdc);

		switch (wParam) {
		case VK_ESCAPE:
			hdc = GetDC(hWnd);
			GetClientRect(hWnd, &rect);
			GetClientRect(hWnd, &rectScroll);
			GetTextMetrics(hdc, &tm);
			rectScroll.top += (tm.tmHeight + 5);
			ScrollWindow(hWnd, 0, -rect.bottom / 25, &rectScroll, &rectScroll);
			ReleaseDC(hWnd, hdc);
		default:
			return 0;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		GetClientRect(hWnd, &rectScroll);
		GetTextMetrics(hdc, &tm);
	
		TextOut(hdc, rect.right * 0/18 +10, rect.bottom * 0/25, szTitleMessage, 7);
		TextOut(hdc, rect.right * 3/18, rect.bottom * 0/25, szTitleKey, 3);
		TextOut(hdc, rect.right * 5 / 18, rect.bottom * 0 / 25, szTitleChar, 4);
		TextOut(hdc, rect.right * 7 / 18, rect.bottom * 0 / 25, szTitleRepeat, 6);
		TextOut(hdc, rect.right * 9 / 18, rect.bottom * 0 / 25, szTitleScan, 4);
		TextOut(hdc, rect.right * 11 / 18, rect.bottom * 0 / 25, szTitleExt, 3);
		TextOut(hdc, rect.right * 13 / 18, rect.bottom * 0 / 25, szTitleAlt, 3);
		TextOut(hdc, rect.right * 15 / 18, rect.bottom * 0 / 25, szTitlePrev, 4);
		TextOut(hdc, rect.right * 17 / 18, rect.bottom * 0 / 25, szTitleTran, 4);

		MoveToEx(hdc, rect.right * 0 / 18 + 10, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 0 / 18 + 10 + tm.tmMaxCharWidth*1 + tm.tmAveCharWidth*6 +3), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 3 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 3 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 5 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 5 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 +1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 7 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 7 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 5 -1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 9 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 9 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 3 -1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 11 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 11/ 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 1 +1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 13 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 13 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 1 +1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 15 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 15 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 +1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 17 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 17 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 +1), rect.bottom * 0 / 25 + tm.tmHeight);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Do you really want to quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
