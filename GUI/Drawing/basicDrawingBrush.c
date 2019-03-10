/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicDrawingBrush.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 10/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Create basic brushs
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 10032019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>
#define BRIGHTNESS (255/5)
//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[] = "Basic Drawing - Brush";
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

	HBRUSH hBrush, hOldBrush;
	//DATA

	//CODE
	switch(message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		for (unsigned short int loopCounter = 1; loopCounter < 7; loopCounter += 2) {
			//Definition HS_BDIAGONAL
			/*Row 1 Rectangle*/
			SetBkColor(hdc, RGB((unsigned short int)(loopCounter / 2) * BRIGHTNESS, (unsigned short int)(loopCounter / 2) * BRIGHTNESS, (unsigned short int)(loopCounter / 2) * BRIGHTNESS));
			hBrush = CreateHatchBrush((unsigned short int)(loopCounter / 2), RGB(0xff - (unsigned short int)(loopCounter / 2) * 85, (loopCounter / 2) *85, 0));
			hOldBrush = SelectObject(hdc, hBrush);
			Rectangle(hdc, rect.right * loopCounter/7, rect.bottom * 1/5, rect.right * (loopCounter +1)/7, rect.bottom * 2/5);
			DeleteObject(SelectObject(hdc, hOldBrush));

			/*Row 2 Rectangle*/
			SetBkColor(hdc, RGB(((unsigned short int)(loopCounter / 2) +3) * BRIGHTNESS, ((unsigned short int)(loopCounter / 2) + 3) * BRIGHTNESS, ((unsigned short int)(loopCounter / 2) + 3) * BRIGHTNESS));
			hBrush = CreateHatchBrush((unsigned short int)(loopCounter / 2)+3, RGB(0, 0xff - (unsigned short int)(loopCounter / 2) * 85, (loopCounter / 2) * 85));
			hOldBrush = SelectObject(hdc, hBrush);
			Rectangle(hdc, rect.right * loopCounter/7, rect.bottom * 3/5, rect.right * (loopCounter +1)/7, rect.bottom * 4/5);
			DeleteObject(SelectObject(hdc, hOldBrush));
		}
		SetBkColor(hdc, RGB(0xff, 0xff, 0xff));
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
