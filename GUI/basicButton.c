/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicButton.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Show different buttons style.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 12042019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <math.h>
#include <windows.h>

struct {
	int iStyle;
	TCHAR* szText;
}button[] = {
	BS_PUSHBUTTON, TEXT("PUSHBUTTON"),
	BS_DEFPUSHBUTTON, TEXT("DEFPUSHBUTTON"),
	BS_CHECKBOX, TEXT("CHECKBOX"),
	BS_AUTOCHECKBOX, TEXT("AUTOCHECKBOX"),
	BS_RADIOBUTTON, TEXT("RADIOBUTTON"),
	BS_3STATE, TEXT("3STATE"),
	BS_AUTO3STATE, TEXT("AUTO3STATE"),
	BS_GROUPBOX, TEXT("GROUPBOX"),
	BS_AUTORADIOBUTTON, TEXT("AUTORADIOBUTTON"),
	BS_OWNERDRAW, TEXT("OWNERDRAW"),
};

//DEFINE
#define NUMOFBUTTON (sizeof button / sizeof button[0])

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[] = "Basic GUI - Button";
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
	//DATA
	HDC hDc;
	PAINTSTRUCT ps;
	static int xCharPos, yCharPos;

	//CODE
	switch (message) {
	case WM_CREATE:
		xCharPos = LOWORD(GetDialogBaseUnits());
		yCharPos = HIWORD(GetDialogBaseUnits());

		for (int loopCounter = 0; loopCounter < NUMOFBUTTON; loopCounter++) {
			CreateWindow(TEXT("button"), button[loopCounter].szText, WS_CHILD | WS_VISIBLE | button[loopCounter].iStyle, xCharPos, yCharPos * (1 + 2 * loopCounter), 20 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)loopCounter, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}
		return 0;

	case WM_COMMAND:
		MessageBox(hWnd, TEXT("Button CLICKED"), TEXT("MESSAGE"), MB_OK | MB_ICONINFORMATION);
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
