/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicChildWndProc.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 20/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Create a child Windows Process.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 20032019    lst97       1      First release
;* 22032019    lst97       2      Add keyboard event
;* Known Issue       :
;*
;|**********************************************************************;
*/
/* Reference */
// https://www.bilibili.com/video/av10340284/?p=42 | https://fishc.com

// Header
#include <windows.h>

// Define
#define BLOCKNUMBER 5

// Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

// DATA
static char szMainAppName[] = "Basic Keyboard&Mouse - Crossing Block";
static char szErrorMessage[] = "This program only run on Windows NT!";
static unsigned int nClientWidth = 640;
static unsigned int nClientHeight = 480;

static unsigned int idFocus;

static char szSubAppName[] = "Sub Class";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {

	//WNDCLASS -> RegisterClass -> CreateWindow -> ShowWindow -> UpdateWindow -> MsgLoop

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;			//Redraws the entire window if a movement or size adjustment changes.
	wndClass.lpfnWndProc = WndProc;									//A pointer to the window procedure.
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;									//A handle to the instance that contains the window procedure for the class.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//A handle to the class background brush.
	wndClass.lpszMenuName = NULL;									//The resource name of the class menu, as the name appears in the resource file.
	wndClass.lpszClassName = szMainAppName;							//The maximum length for lpszClassName is 256.

	if (!RegisterClass(&wndClass)) {								//Value is zero if Register FAIL (!0) = TRUE;
		MessageBox(NULL, szErrorMessage, szMainAppName, MB_ICONERROR);
		return 0;
	}
	wndClass.lpfnWndProc = ChildWndProc;
	wndClass.cbWndExtra = sizeof(long);
	wndClass.hIcon = NULL;
	wndClass.lpszClassName = szSubAppName;

	RegisterClass(&wndClass);

	HWND hWnd;
	hWnd = CreateWindow(szMainAppName, szMainAppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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
	static HWND hWndChildWndProc[BLOCKNUMBER][BLOCKNUMBER];
	unsigned int cxBlockWidth, cyBlockHeight, xBlockPos, yBlockPos;

	switch(message){
	case WM_CREATE:
		for (xBlockPos = 0; xBlockPos < BLOCKNUMBER; xBlockPos++) {
			for (yBlockPos = 0; yBlockPos < BLOCKNUMBER; yBlockPos++) {
				hWndChildWndProc[xBlockPos][yBlockPos] = CreateWindow(szSubAppName, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd,
					(HMENU)(yBlockPos << 8 | xBlockPos), (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE), NULL);
			}
		}

		return 0;

	case WM_SIZE:
		cxBlockWidth = LOWORD(lParam) / BLOCKNUMBER;
		cyBlockHeight = HIWORD(lParam) / BLOCKNUMBER;

		for (xBlockPos = 0; xBlockPos < BLOCKNUMBER; xBlockPos++) {
			for (yBlockPos = 0; yBlockPos < BLOCKNUMBER; yBlockPos++) {
				MoveWindow(hWndChildWndProc[xBlockPos][yBlockPos], xBlockPos * cxBlockWidth, yBlockPos * cyBlockHeight, cxBlockWidth, cyBlockHeight, TRUE);
			}
		}

	case WM_LBUTTONDOWN:
		MessageBeep(0);
		return 0;

	case WM_SETFOCUS:
		SetFocus(GetDlgItem(hWnd, idFocus));
		return 0;

	case WM_KEYDOWN:
		xBlockPos = idFocus & 0xFF;
		yBlockPos = idFocus >> 8;

		switch (wParam) {
		case VK_UP:		yBlockPos--; break;
		case VK_DOWN:	yBlockPos++; break;
		case VK_LEFT:	xBlockPos--; break;
		case VK_RIGHT:	xBlockPos++; break;
		case VK_HOME:	xBlockPos = yBlockPos = 0; break;
		case VK_END:	xBlockPos = yBlockPos = BLOCKNUMBER - 1; break;
		default: return 0;
		}

		xBlockPos = (xBlockPos + BLOCKNUMBER) % BLOCKNUMBER;
		yBlockPos = (yBlockPos + BLOCKNUMBER) % BLOCKNUMBER;

		idFocus = yBlockPos << 8 | xBlockPos;
		SetFocus(GetDlgItem(hWnd, idFocus));
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Do you really want to QUIT?"), TEXT("MESSAGE"), MB_YESNO | MB_ICONQUESTION)) {
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

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message) {

	case WM_CREATE:
		SetWindowLongPtr(hWnd, 0, 0);
		return 0;

	case WM_KEYDOWN:
		if (wParam != VK_RETURN && wParam != VK_SPACE) {
			SendMessage(GetParent(hWnd), message, wParam, lParam);
			return 0;
		}

	case WM_LBUTTONDOWN:
		SetWindowLongPtr(hWnd, 0, 1 ^ GetWindowLongPtr(hWnd, 0));
		SetFocus(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_SETFOCUS:
		idFocus = GetWindowLongPtr(hWnd, GWL_ID);
	case WM_KILLFOCUS:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);
		Rectangle(hdc, 0, 0, rect.right, rect.bottom);

		if (GetWindowLongPtr(hWnd, 0)) {
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, rect.right, rect.bottom);
			MoveToEx(hdc, 0, rect.bottom, NULL);
			LineTo(hdc, rect.right, 0);
		}

		if (hWnd == GetFocus()) {
			rect.left += rect.right / 10;
			rect.right -= rect.left;
			rect.top += rect.bottom / 10;
			rect.bottom -= rect.top;

			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			SelectObject(hdc, CreatePen(PS_DASH, 0, 0));
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
		}
		EndPaint(hWnd, &ps);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
