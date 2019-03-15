/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : keyLogger.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 10/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Logging user keyboard event.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 10032019    lst97       1      First release
;* 11032019    lst97       1      add more code but dont understand some concept, goto next section, come back when finish.
;* 15032019    lst97       2      Fully functional.
;* 
;* Known Issue       :
;* 1. Incorrect key when program first start, because of no memory clear.
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
	static PMSG pmsg;	//must static?

	//DATA
	static char szTitleMessage[] = "Message";
	static char szTitleKey[] = "Key";
	static char szTitleChar[] = "Char";
	static char szTitleRepeat[] = "Repeat";
	static char szTitleScan[] = "Scan";
	static char szTitleExt[] = "Ext";
	static char szTitleAlt[] = "ALT";
	static char szTitlePrev[] = "Prev";
	static char szTitleTran[] = "Tran";

	static char *szMessagePool[] = { TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"), TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"), TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"), TEXT("WM_SYSCHAR"), TEXT("WM_SYSDEADCHAR") };
	static unsigned int szKey = 0;
	static unsigned int szRepeat = 0;
	static unsigned int szScan = 0;
	static char szUp[4] = "Up  ";
	static char szDown[4] = "Down";
	static char szTrue[5] = "True ";
	static char szFalse[5] = "False";

	static unsigned int strLength = 0;

	//CODE
	switch (message) {
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		GetClientRect(hWnd, &rectScroll);
		ReleaseDC(hWnd, hdc);

		pmsg = malloc(rect.bottom / 25 * sizeof(MSG));

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:

		pmsg[0].hwnd = hWnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		// Scrolling effect for each key
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		GetClientRect(hWnd, &rectScroll);
		GetTextMetrics(hdc, &tm);
		rectScroll.top += (tm.tmHeight +14);
		ScrollWindow(hWnd, 0, -rect.bottom / 25, &rectScroll, &rectScroll);
		ReleaseDC(hWnd, hdc);

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		GetClientRect(hWnd, &rectScroll);
		GetTextMetrics(hdc, &tm);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

		// Draw Title
		TextOut(hdc, rect.right * 0 / 18 + 10, rect.bottom * 0 / 25, szTitleMessage, 7);
		TextOut(hdc, rect.right * 4 / 18, rect.bottom * 0 / 25, szTitleKey, 3);
		TextOut(hdc, rect.right * 5 / 18, rect.bottom * 0 / 25, szTitleChar, 4);
		TextOut(hdc, rect.right * 7 / 18, rect.bottom * 0 / 25, szTitleRepeat, 6);
		TextOut(hdc, rect.right * 9 / 18, rect.bottom * 0 / 25, szTitleScan, 4);
		TextOut(hdc, rect.right * 11 / 18, rect.bottom * 0 / 25, szTitleExt, 3);
		TextOut(hdc, rect.right * 13 / 18, rect.bottom * 0 / 25, szTitleAlt, 3);
		TextOut(hdc, rect.right * 15 / 18, rect.bottom * 0 / 25, szTitlePrev, 4);
		TextOut(hdc, rect.right * 17 / 18, rect.bottom * 0 / 25, szTitleTran, 4);

		// Draw Title Underline
		MoveToEx(hdc, rect.right * 0 / 18 + 10, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 0 / 18 + 10 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 6 + 2), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 4 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 4 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 1 + 2), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 5 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 5 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 + 3), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 7 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 7 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 5 - 1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 9 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 9 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 3 - 1), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 11 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 11 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 1 + 2), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 13 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 13 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 1 + 2), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 15 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 15 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 + 4), rect.bottom * 0 / 25 + tm.tmHeight);

		MoveToEx(hdc, rect.right * 17 / 18, rect.bottom * 0 / 25 + tm.tmHeight, NULL);
		LineTo(hdc, (rect.right * 17 / 18 + tm.tmMaxCharWidth * 1 + tm.tmAveCharWidth * 2 + 4), rect.bottom * 0 / 25 + tm.tmHeight);

		// Output
		/*
		#define WM_KEYFIRST                     0x0100
		#define WM_KEYDOWN                      0x0100
		#define WM_KEYUP                        0x0101
		#define WM_CHAR                         0x0102
		#define WM_DEADCHAR                     0x0103
		#define WM_SYSKEYDOWN                   0x0104
		#define WM_SYSKEYUP                     0x0105
		#define WM_SYSCHAR                      0x0106
		#define WM_SYSDEADCHAR                  0x0107
		*/
		StringCchLength(szMessagePool[pmsg[0].message - 0x0100], 16, &strLength);
		TextOut(hdc, rect.right * 0 / 18 + 10, rect.bottom * 24 / 25, szMessagePool[pmsg[0].message - 0x0100], strLength);		// -0x0100 = szMessagePool[1] || szMessagePool[2] || szMessagePool[3] e.t.c.
		szKey = pmsg[0].wParam;

		// If WM_CHAR event happen, display will be little bit different. Different (szKey)
		0x00000102 == pmsg[0].message ? TextOut(hdc, rect.right * 5 / 18, rect.bottom * 24 / 25, (LPCSTR)(&szKey), 1) : TextOut(hdc, rect.right * 4 / 18, rect.bottom * 24 / 25, (LPCSTR)(&szKey), 1);
		
		// +0x30 for single int to string
		szRepeat = LOWORD(pmsg[0].lParam) + 0x30;
		TextOut(hdc, rect.right * 7 / 18, rect.bottom * 24 / 25, (LPCSTR)(&szRepeat), 1);

		// Scan code - HIWORD(pmsg[0].lParam) storage scan code. (unsigned int)
		char szBufferTemp[3];
		StringCchPrintf(szBufferTemp, 3, TEXT("%d"), HIWORD(pmsg[0].lParam) & 0xFF);
		TextOut(hdc, rect.right * 9 / 18, rect.bottom * 24 / 25, szBufferTemp, 2);

		// Other 4 cat
		TextOut(hdc, rect.right * 11 / 18, rect.bottom * 24 / 25, 0x01000000 & pmsg[0].lParam ? szTrue : szFalse, 5);
		TextOut(hdc, rect.right * 13 / 18, rect.bottom * 24 / 25, 0x20000000 & pmsg[0].lParam ? szTrue : szFalse, 5);
		TextOut(hdc, rect.right * 15 / 18, rect.bottom * 24 / 25, 0x40000000 & pmsg[0].lParam ? szUp : szDown, 4);
		TextOut(hdc, rect.right * 17 / 18, rect.bottom * 24 / 25, 0x80000000 & pmsg[0].lParam ? szUp : szDown, 4);
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
