/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : bcNotepad.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 16/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Before Centre Notepad, no save, clear paint when minmize.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 16032019    lst97       1      First release
;* 
;* Known Issue       :
;*
;|**********************************************************************;
*/
/* Reference */
// https://www.bilibili.com/video/av10340284/?p=36 | https://fishc.com

// Header
#include <windows.h>

// Define
#define BUFFER(loopPosX, loopPosY) *(paintBuffer + loopPosY * cxBuffer + loopPosX)	// take the word in x,y pos

// Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// DATA
static char szAppName[] = "Basic Keyboard&Mouse - BC Notepad";
static char szErrorMessage[] = "This program only run on Windows NT!";
static unsigned int nClientWidth = 640;
static unsigned int nClientHeight = 480;

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
	// DATA
	//static DWORD dwCharSet = DEFAULT_CHARSET;
	static unsigned int cxCharWidth, cyCharHeight, cxClientWidth, cyClientHeight, cxBuffer, cyBuffer, xCaretPos, yCaretPos;
	static TCHAR *paintBuffer = NULL;
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;

	// CODE
	switch(message){
	case WM_CREATE:
		hdc = GetDC(hWnd);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxCharWidth = tm.tmAveCharWidth;
		cyCharHeight = tm.tmHeight;

		ReleaseDC(hWnd, hdc);

	case WM_SIZE:
		// Resize clean all data
		if (message == WM_SIZE) {
			cxClientWidth = LOWORD(lParam);
			cyClientHeight = HIWORD(lParam);
		}

		// Get how many Col and Row in the paint
		cxBuffer = max(1, cxClientWidth / cxCharWidth);
		cyBuffer = max(1, cyClientHeight / cyCharHeight);

		if (paintBuffer != NULL) free(paintBuffer);

		// Allocate as many as memory size in paint
		paintBuffer = (TCHAR *)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));

		// Fill full of spacebar
		for (unsigned int loopPosY = 0; loopPosY < cyBuffer; loopPosY++) {
			for (unsigned int loopPosX = 0; loopPosX < cxBuffer; loopPosX++) {
				BUFFER(loopPosX, loopPosY) = 0x0020;
			}
		}
		xCaretPos = 0; yCaretPos = 0;

		if (hWnd == GetFocus()) SetCaretPos(xCaretPos, yCaretPos);

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_SETFOCUS:
		CreateCaret(hWnd, NULL, 1, cyCharHeight);
		SetCaretPos(xCaretPos , yCaretPos);
		ShowCaret(hWnd);
		return 0;

	case WM_KILLFOCUS:
		HideCaret(hWnd);
		DestroyCaret();
		return 0;

	// Keyboard Event
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_HOME:
			xCaretPos = 0;
			break;
		case VK_END:
			xCaretPos = cxBuffer - 1;
			break;

		case VK_PRIOR:
			yCaretPos = 0;
			break;
		case VK_NEXT:
			yCaretPos = cyBuffer - 1;

		case VK_LEFT:
			xCaretPos = max(xCaretPos - 1, 0);
			break;
		case VK_RIGHT:
			xCaretPos = min(xCaretPos + 1, cxBuffer - 1);
			break;
		case VK_UP:
			yCaretPos = max(yCaretPos - 1, 0);
			break;
		case VK_DOWN:
			yCaretPos = min(yCaretPos + 1, cyBuffer - 1);
			break;

		case VK_DELETE:
			for (unsigned int loopPosX = xCaretPos; loopPosX < cxBuffer - 1; loopPosX++) {
				BUFFER(loopPosX, yCaretPos) = BUFFER(loopPosX + 1, yCaretPos);
			}
			BUFFER(cxBuffer -1, yCaretPos) = 0x0020;

			HideCaret(hWnd);
			hdc = GetDC(hWnd);

			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			TextOut(hdc, xCaretPos * cxCharWidth, yCaretPos * cyCharHeight, &BUFFER(xCaretPos, yCaretPos), cxBuffer - xCaretPos);

			ReleaseDC(hWnd, hdc);
			ShowCaret(hWnd);
			break;
		}

		SetCaretPos(xCaretPos * cxCharWidth, yCaretPos * cyCharHeight);
		return 0;

	case WM_CHAR:
		for (unsigned int loopCounter = 0; loopCounter < (int)LOWORD(lParam); loopCounter++){
			switch (wParam) {
			case '\b':
				if (xCaretPos > 0) {
					xCaretPos--;
					SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 1);
				}
				break;

			case '\t':
				do {
					SendMessage(hWnd, WM_CHAR, 0x0020, 1);
				} while (xCaretPos % 8 != 0);
				break;

			case '\n':
				if (++yCaretPos == cyBuffer) {
					yCaretPos = 0;
				}
				break;

			case '\r':
				xCaretPos = 0;
				if (++yCaretPos == cyBuffer) {
					yCaretPos = 0;
				}
				break;

			case '\x1B':
				// Write to buffer first then repaint
				for (unsigned int loopPosY = 0; loopPosY < cyBuffer; loopPosY++) {
					for (unsigned int loopPosX = 0; loopPosX  < cxBuffer; loopPosX++) {
						BUFFER(loopPosX, loopPosY) = 0x0020;
					}
				}
				xCaretPos = 0;
				yCaretPos = 0;

				InvalidateRect(hWnd, NULL, FALSE);
				break;

			default:
				// Show char
				BUFFER(xCaretPos, yCaretPos) = (TCHAR)wParam;
				HideCaret(hWnd);
				hdc = GetDC(hWnd);

				SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
				TextOut(hdc, xCaretPos * cxCharWidth, yCaretPos * cyCharHeight, &BUFFER(xCaretPos, yCaretPos), 1);

				ReleaseDC(hWnd, hdc);
				ShowCaret(hWnd);

				if (++xCaretPos == cxBuffer) {
					xCaretPos = 0;
					if (++yCaretPos == cyBuffer) {
						yCaretPos = 0;
					}
				}
				break;
			}
		}

		SetCaretPos(xCaretPos * cxCharWidth, yCaretPos * cyCharHeight);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// For a Roll
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		for (unsigned int loopPosY = 0; loopPosY < cyBuffer; loopPosY++) {
			TextOut(hdc, 0, loopPosY * cyCharHeight, &BUFFER(0, loopPosY), cxBuffer);
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
