// https://www.bilibili.com/video/av10340284/?p=35
/* Keylogger Reference*/

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
	static int cxClientMaxPos, cyClientMaxPos, cxClientPos, cyClientPos, cxCharWidth, cyCharHeight;
	static int cLinesMax, cLines;
	static PMSG pmsg;
	static RECT rectScroll;
	static TCHAR szTitle[] = TEXT("Message      Key      Char      Repeat Scan Ext ALT Prev TRAN");
	static TCHAR szUnderline[] = TEXT("______      ___      ____      ______ ____ ___ ___ ____ ____");
	static TCHAR *szFormat[2] = {TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"), TEXT("%-13s            0x%04X%1s%c ^6u %4d %3s %3s %4s %4s")};

	static TCHAR *szYes = TEXT("Yes");
	static TCHAR *szNo = TEXT("No");
	static TCHAR *szDown = TEXT("Down");
	static TCHAR *szUp = TEXT("Up");
	static TCHAR *szMessage[] = { TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"), TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"), TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"), TEXT("WM_SYSCHAR"), TEXT("WM_SYSDEADCHAR") };

	HDC hdc;
	int iType, iTarget;
	PAINTSTRUCT ps;
	TCHAR szBuffer[128], szKeyName[32];
	TEXTMETRIC tm;

	switch(message){
	case WM_CREATE:
	case WM_DISPLAYCHANGE:
		cxClientMaxPos = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMaxPos = GetSystemMetrics(SM_CYMAXIMIZED);

		hdc = GetDC(hWnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxCharWidth = tm.tmAveCharWidth;
		cyCharHeight = tm.tmHeight;

		ReleaseDC(hWnd, hdc);

		if (pmsg) {
			free(pmsg);
		}

		cLinesMax = cyClientMaxPos / cyCharHeight;
		pmsg = malloc(cLinesMax * sizeof(MSG));

	case WM_SIZE:
		if (message == WM_SIZE) {
			cxClientPos = LOWORD(lParam);
			cyClientPos = HIWORD(lParam);
		}

		rectScroll.left = 0;
		rectScroll.right = cxClientPos;
		rectScroll.top = cyCharHeight;
		rectScroll.bottom = cyCharHeight * (cyClientPos / cyCharHeight);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		for (int loopCounter = cLinesMax - 1; loopCounter > 0; loopCounter--) {
			pmsg[loopCounter] = pmsg[loopCounter - 1];
		}
		pmsg[0].hwnd = hWnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		cLines = min(cLines + 1, cLinesMax);
		ScrollWindow(hWnd, 0, -cyCharHeight, &rectScroll, &rectScroll);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc, TRANSPARENT);
		StringCchLength(szTitle, sizeof(szTitle), &iTarget);
		TextOut(hdc, 0, 0, szTitle, iTarget);
		StringCchLength(szUnderline, sizeof(szUnderline), &iTarget);
		TextOut(hdc, 0, 0, szUnderline, iTarget);
		for (int loopCounter = 0; loopCounter < min(cLines, cyClientPos / cyCharHeight - 1); loopCounter++) {
			iType = pmsg[loopCounter].message == WM_CHAR || pmsg[loopCounter].message == WM_SYSCHAR || pmsg[loopCounter].message == WM_DEADCHAR || pmsg[loopCounter].message == WM_SYSDEADCHAR;

			GetKeyNameText(pmsg[loopCounter].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
			StringCchPrintf(szBuffer, sizeof(szBuffer), szFormat[iType], szMessage[pmsg[loopCounter].message - WM_KEYFIRST], pmsg[loopCounter].wParam, (PTSTR)(iType ? TEXT(" ") : szKeyName), (TCHAR)(iType ? pmsg[loopCounter].wParam : ' '), LOWORD(pmsg[loopCounter].lParam), HIWORD(pmsg[loopCounter].lParam) & 0xFF,
				0x01000000 & pmsg[loopCounter].lParam ? szYes : szNo,
				0x20000000 & pmsg[loopCounter].lParam ? szYes : szNo,
				0x40000000 & pmsg[loopCounter].lParam ? szDown : szUp,
				0x80000000 & pmsg[loopCounter].lParam ? szUp : szDown);

			StringCchLength(szBuffer, sizeof(szAppName), &iTarget);
			TextOut(hdc, 0, (cyClientPos / cyCharHeight - 1 - loopCounter) * cyCharHeight, szBuffer, iTarget);
		}
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
