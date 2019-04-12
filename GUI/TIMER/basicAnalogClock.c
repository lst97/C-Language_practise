/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicAnalogClock.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 12/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Basic analog clock
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 12042019    lst97       1      First release
;*
;* Known Issue       :
;* 1. Don't understand yet review later.
;*
;|**********************************************************************;
*/

#include <math.h>
#include <windows.h>

//DEFINE
#define ID_TIMER 1
#define TWOPI (2 * 3.141592654)

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetIsotropic(HDC hDc, int xClientPos, int yClientPos);
void RotatePoint(POINT pt[], int iNumber, int iAngle);
void DrawClockLable(HDC hDc);
void DrawClockHands(HDC hDc, SYSTEMTIME* pSystemTime, BOOL boolChange);


//DATA
static char szAppName[] = "Basic Timer - Analog Clock";
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

void SetIsotropic(HDC hDc, int xClientPos, int yClientPos) {
	SetMapMode(hDc, MM_ISOTROPIC);
	SetWindowExtEx(hDc, 1000, 1000, NULL);
	SetViewportExtEx(hDc, xClientPos / 2, -yClientPos / 2, NULL);
	SetViewportOrgEx(hDc, xClientPos / 2, yClientPos / 2, NULL);
}

void RotatePoint(POINT pt[], int iNumber, int iAngle) {
	int loopCounter;
	POINT ptTemp;
	
	for (loopCounter = 0; loopCounter < iNumber; loopCounter++) {
		ptTemp.x = (int)(pt[loopCounter].x * cos(TWOPI * iAngle / 360) + pt[loopCounter].y * sin(TWOPI * iAngle / 360));
		ptTemp.y = (int)(pt[loopCounter].y * cos(TWOPI * iAngle / 360) + pt[loopCounter].x * sin(TWOPI * iAngle / 360));
		pt[loopCounter] = ptTemp;
	}
}

void DrawClockLable(HDC hDc) {
	int iAngle;
	POINT pt[3];

	for (iAngle = 0; iAngle < 360; iAngle += 6) {
		pt[0].x = 0;
		pt[0].y = 900;

		RotatePoint(pt, 1, iAngle);

		pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;

		pt[0].x -= pt[2].x / 2;
		pt[0].y -= pt[2].y / 2;
		
		pt[1].x = pt[0].x + pt[2].x;
		pt[1].y = pt[0].y + pt[2].y;

		SelectObject(hDc, GetStockObject(BLACK_BRUSH));
		
		Ellipse(hDc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);

	}
}

void DrawClockHands(HDC hDc, SYSTEMTIME * pSystemTime, BOOL boolChange) {
	static POINT pt[3][5] = {
		0, -150, 100, 0, 0, 600, -100, 0, 0, -150,
		0, -200, 50, 0, 0, 800, -50, 0, 0, -200,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 800
	};

	int loopCounter, iAngle[3];
	POINT ptTemp[3][5];

	iAngle[0] = (pSystemTime->wHour * 30) % 360 + pSystemTime->wMinute / 2;
	iAngle[1] = pSystemTime->wMinute * 6;
	iAngle[2] = pSystemTime->wSecond * 6;

	memcpy(ptTemp, pt, sizeof(pt));

	for (loopCounter = boolChange ? 0 : 2; loopCounter < 3; loopCounter++) {
		RotatePoint(ptTemp[loopCounter], 5, iAngle[loopCounter]);

		Polyline(hDc, ptTemp[loopCounter], 5);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA
	HDC hDc;
	PAINTSTRUCT ps;
	BOOL boolChange;
	SYSTEMTIME st;
	static int xClientPos, yClientPos;
	static SYSTEMTIME stPrevious;

	//CODE
	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, ID_TIMER, 1000, NULL);
		GetLocalTime(&st);
		stPrevious = st;
		return 0;

	case WM_SIZE:
		xClientPos = LOWORD(lParam);
		yClientPos = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		GetLocalTime(&st);
		boolChange = st.wHour != stPrevious.wHour || st.wMinute != stPrevious.wMinute;

		hDc = GetDC(hWnd);
		SetIsotropic(hDc, xClientPos, yClientPos);

		SelectObject(hDc, GetStockObject(WHITE_PEN));
		DrawClockHands(hDc, &stPrevious, boolChange);

		SelectObject(hDc, GetStockObject(BLACK_PEN));
		DrawClockHands(hDc, &st, TRUE);

		ReleaseDC(hWnd, hDc);

		stPrevious = st;
		return 0;

	case WM_PAINT:
		hDc = BeginPaint(hWnd, &ps);
		
		SetIsotropic(hDc, xClientPos, yClientPos);
		DrawClockLable(hDc);
		DrawClockHands(hDc, &stPrevious, TRUE);
		
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Do you really want to quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			KillTimer(hWnd, ID_TIMER);
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
