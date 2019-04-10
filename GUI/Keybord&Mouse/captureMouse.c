/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : captureMouse.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 10/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Drawing Rectangle.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 10042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windowsx.h>
#include <windows.h>

//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawRecOutline(HWND hWnd, POINT ptStart, POINT ptEnd);

//DATA
static char szAppName[] = "Basic Keyboard&Mouse - Capture the Mouse";
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

void DrawRecOutline(HWND hWnd, POINT ptStart, POINT ptEnd) {
	HDC hDc;
	hDc = GetDC(hWnd);

	SetROP2(hDc, R2_NOT);
	SelectObject(hDc, GetStockObject(NULL_BRUSH));
	Rectangle(hDc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

	ReleaseDC(hWnd, hDc);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	//DATA
	HDC hDc;
	PAINTSTRUCT ps;
	static BOOL boolDrawing, boolValidRec;
	static POINT ptStart, ptEnd, ptRecStart, ptRecEnd;

	//CODE
	switch(message){
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(NULL, IDC_CROSS));								// The cursor will flashing when you move, as it temporarily, set permanent recommened.
		SetClassLong(hWnd, GCL_HCURSOR, LoadCursor(NULL, IDC_CROSS));		// Set permanent 

		ptStart.x = ptEnd.x = GET_X_LPARAM(lParam);
		ptStart.y = ptEnd.y = GET_Y_LPARAM(lParam);

		SetCapture(hWnd);													// Capture cursor when outside the windows, remember to release.

		DrawRecOutline(hWnd, ptStart, ptEnd);

		boolDrawing = TRUE;
		return 0;

	case WM_MOUSEMOVE:
		
		if (boolDrawing) {
			DrawRecOutline(hWnd, ptStart, ptEnd);

			ptEnd.x = GET_X_LPARAM(lParam);
			ptEnd.y = GET_Y_LPARAM(lParam);
			DrawRecOutline(hWnd, ptStart, ptEnd);
		}
		return 0;

	case WM_LBUTTONUP:
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetClassLong(hWnd, GCL_HCURSOR, LoadCursor(NULL, IDC_ARROW));
		if (boolDrawing) {
			DrawRecOutline(hWnd, ptStart, ptEnd);

			ptRecStart = ptStart;
			ptRecEnd.x = GET_X_LPARAM(lParam);
			ptRecEnd.y = GET_Y_LPARAM(lParam);

			ReleaseCapture();

			boolDrawing = FALSE;
			boolValidRec = TRUE;

			InvalidateRect(hWnd, NULL, TRUE);
		}

	case WM_PAINT:
		hDc = BeginPaint(hWnd, &ps);
		if (boolValidRec) {
			SelectObject(hDc, GetStockObject(BLACK_BRUSH));
			Rectangle(hDc, ptRecStart.x, ptRecStart.y, ptRecEnd.x, ptRecEnd.y);
		}

		if (boolDrawing) {
			SetROP2(hDc, R2_NOT);
			SelectObject(hDc, GetStockObject(NULL_BRUSH));
			Rectangle(hDc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
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
