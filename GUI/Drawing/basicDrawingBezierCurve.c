/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicDrawingBezierCurve.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 05/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Create BezierCurve 3P
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 05032019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>

//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[] = "Basic Drawing - Poly line [Bezier]";
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

	ShowWindow(hWnd, nCmdShow);

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

	//DATA
	static POINT apt[4];
	static int cxClientPos, cyClientPos;

	//CODE
	switch(message) {
	case WM_CREATE:

		MessageBox(hWnd, TEXT("Left Click\t\t: setting posistion 1\nRight Click\t: setting posistion 2"), TEXT("How to play"), MB_OK | MB_ICONINFORMATION);
		return 0;

	case WM_SIZE:
		cxClientPos = LOWORD(lParam);
		cyClientPos = HIWORD(lParam);

		apt[0].x = cxClientPos / 4;
		apt[0].y = cyClientPos / 2;

		apt[1].x = cxClientPos / 2;
		apt[1].y = cyClientPos / 4;

		apt[2].x = cxClientPos / 2;
		apt[2].y = cyClientPos * 3 / 4;

		apt[3].x = cxClientPos / 2;
		apt[3].y = cyClientPos / 4;

		return 0;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);

		SelectObject(hdc, GetStockObject(WHITE_PEN));

		PolyBezier(hdc, apt, 4);

		apt[1].x = LOWORD(lParam);
		apt[1].y = HIWORD(lParam);

		SelectObject(hdc, GetStockObject(BLACK_PEN));

		PolyBezier(hdc, apt, 4);

		ReleaseDC(hWnd, hdc);

		return 0;

	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);

		SelectObject(hdc, GetStockObject(WHITE_PEN));
		PolyBezier(hdc, apt, 4);

		apt[2].x = LOWORD(lParam);
		apt[2].y = HIWORD(lParam);

		SelectObject(hdc, GetStockObject(BLACK_PEN));
		PolyBezier(hdc, apt, 4);

		ReleaseDC(hWnd, hdc);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		PolyBezier(hdc, apt, 4);

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
