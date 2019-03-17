/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicMouseEventBlockCross.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 18/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Crossing block, game?
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 18032019    lst97       1      First release
;* 
;* Known Issue       :
;*
;|**********************************************************************;
*/
/* Reference */
// https://www.bilibili.com/video/av10340284/?p=39 | https://fishc.com

// Header
#include <windows.h>
#include <windowsx.h>

// Define
#define BLOCKNUMBER 5

// Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// DATA
static char szAppName[] = "Basic Keyboard&Mouse - Mouse Event - Double Click";
static char szErrorMessage[] = "This program only run on Windows NT!";
static unsigned int nClientWidth = 640;
static unsigned int nClientHeight = 480;

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
	static BOOL boolState[BLOCKNUMBER][BLOCKNUMBER];
	static unsigned int cxBlockWidth, cyBlockHeight;
	HDC hDc;
	unsigned int xBlockPos, yBlockPos;
	PAINTSTRUCT ps;
	RECT rect;

	// CODE
	switch(message){
	case WM_SIZE:
		cxBlockWidth = GET_X_LPARAM(lParam) / BLOCKNUMBER;
		cyBlockHeight = GET_Y_LPARAM(lParam) / BLOCKNUMBER;
		return 0;

	case WM_LBUTTONDOWN:
		xBlockPos = GET_X_LPARAM(lParam) / cxBlockWidth;
		yBlockPos = GET_Y_LPARAM(lParam) / cyBlockHeight;

		if (xBlockPos < BLOCKNUMBER && yBlockPos < BLOCKNUMBER) {
			boolState[xBlockPos][yBlockPos] ^= 1;
			rect.left = xBlockPos * cxBlockWidth;
			rect.top = yBlockPos * cyBlockHeight;
			rect.right = (xBlockPos + 1) * cxBlockWidth;
			rect.bottom = (yBlockPos + 1) * cyBlockHeight;
			InvalidateRect(hWnd, &rect, FALSE);

		}else MessageBeep(0);
		return 0;

	case WM_PAINT:
		hDc = BeginPaint(hWnd, &ps);

		for (xBlockPos = 0; xBlockPos < BLOCKNUMBER; xBlockPos++) {
			for (yBlockPos = 0; yBlockPos < BLOCKNUMBER; yBlockPos++) {
				Rectangle(hDc, xBlockPos * cxBlockWidth, yBlockPos * cyBlockHeight, (xBlockPos + 1) * cxBlockWidth, (yBlockPos + 1) * cyBlockHeight);

				// True than paint cross
				if (boolState[xBlockPos][yBlockPos]) {
					MoveToEx(hDc, xBlockPos * cxBlockWidth, yBlockPos * cyBlockHeight, NULL);
					LineTo(hDc, (xBlockPos + 1) * cxBlockWidth, (yBlockPos + 1) * cyBlockHeight);
					MoveToEx(hDc, xBlockPos * cxBlockWidth, (yBlockPos + 1) * cyBlockHeight, NULL);
					LineTo(hDc, (xBlockPos + 1) * cxBlockWidth, (yBlockPos) * cyBlockHeight);
				}
			}
		}

		EndPaint(hWnd, &ps);
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
