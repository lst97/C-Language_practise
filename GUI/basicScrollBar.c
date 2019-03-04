/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicScrollBar.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 01/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Scroll Bar template - using scrollWindow
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 01032019    lst97       1      First release
;* 04032019    lst97       2      Now can show basic scroll function
;* 04032019    lst97       3      CALLBACK rewrote, using scrollwindow();
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#define LINES 28 

#include <windows.h>
#include "strsafe.h"

//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA:
static char szAppName[] = "Basic Scroll Bar";
static char szErrorMessage[] = "This program only run on Windows NT!";
static int nClientWidth = 640;
static int nClientHeight = 480;

static char *szString[] = {
			TEXT("anteater"), TEXT("bear"), TEXT("cougar"),
			TEXT("dingo"), TEXT("elephant"), TEXT("falcon"),
			TEXT("gazelle"), TEXT("hyena"), TEXT("iguana"),
			TEXT("jackal"), TEXT("kangaroo"), TEXT("llama"),
			TEXT("moose"), TEXT("newt"), TEXT("octopus"),
			TEXT("penguin"), TEXT("quail"), TEXT("rat"),
			TEXT("squid"), TEXT("tortoise"), TEXT("urus"),
			TEXT("vole"), TEXT("walrus"), TEXT("xylophone"),
			TEXT("yak"), TEXT("zebra"),
			TEXT("This line contains words, but no character. Go figure."),
			TEXT("")
};

//hInstance: Handle;
//hPrevInstance: Always NULL (A handle to the previous instance of the application);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {

	//WNDCLASS -> RegisterClass -> CreateWindow -> ShowWindow -> UpdateWindow -> MsgLoop

	/*
	typedef struct tagWNDCLASSA {
	  UINT      style;
	  WNDPROC   lpfnWndProc;
	  int       cbClsExtra;
	  int       cbWndExtra;
	  HINSTANCE hInstance;
	  HICON     hIcon;
	  HCURSOR   hCursor;
	  HBRUSH    hbrBackground;
	  LPCSTR    lpszMenuName;
	  LPCSTR    lpszClassName;
	} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;
	*/

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
	/*
	HWND CreateWindow(
	  LPCWSTR   lpClassName,
	  LPCWSTR   lpWindowName,
	  DWORD     dwStyle,
	  int       X,
	  int       Y,
	  int       nWidth,
	  int       nHeight,
	  HWND      hWndParent,
	  HMENU     hMenu,
	  HINSTANCE hInstance,
	  LPVOID    lpParam
	);
	*/
	//If the function succeeds, the return value is a handle to the new window, NULL if fails.
	hWnd = CreateWindow(szAppName, szAppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, nClientWidth, nClientHeight, NULL, NULL, hInstance, NULL);

	//nCmdShow	控制窗口如何显示，如果发送应用程序的程序提供了 STARTUPINFO 结构，则应用程序第一次调用 ShowWindow 时该参数被忽略。否则，在第一次调用 ShowWindow 函数时，该值应为在函数 WinMain 中 nCmdShow 参数。
	//1. 如果窗口之前可见，则返回值为非 0；
	//2. 如果窗口之前被隐藏，则返回值为 0。
	ShowWindow(hWnd, nCmdShow);

	//UpdateWindow 函数绕过应用程序的消息队列，直接发送 WM_PAINT 消息给指定窗口的窗口过程。
	UpdateWindow(hWnd);
	MSG msg;
	/*
	- - - - - [hWnd] - - - - -
	1. 需要获取消息的窗口的句柄，该窗口必须属于当前线程
	2. 当其值是 NULL 时，将获取所有的当前线程的窗口消息和线程消息
	3. 当其值是 -1 时，只获取当前线程消息
	*/
	while (GetMessage(&msg, NULL, 0, 0)) {
		//TranslateMessage 函数将虚拟键消息转换为字符消息，字符消息被寄送到当前线程的消息队列里。当下一次线程调用函数 GetMessage 或 PeekMessage 时被读出。
		TranslateMessage(&msg);

		//DispatchMessage 函数分派一个消息给窗口过程（回调函数），通常该消息从 GetMessage 函数获得。Windows 的控制权在该函数交给了应用程序。
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	SCROLLINFO si;
	/*typedef struct tagSCROLLINFO {
		UINT cbSize;
		UINT fMask;
		int  nMin;
		int  nMax;
		UINT nPage;
		int  nPos;
		int  nTrackPos;
	} SCROLLINFO, *LPSCROLLINFO;*/
	HRESULT hr;

	//DATA
	static int xClientPos, yClientPos;
	static int xClientPosMax;					//Add a scroll bar beyond this width

	static int xCharPos, yCharPos;
	static int xUpper;							//The horizontal width of the uppercase character of the font

	static int xScrollPos, yScrollPos;

	int xStringPos, yStringPos;

	unsigned int faFirstLine, faLastLine;		//The first line & last line to be repaint

	unsigned int strLength;

	//CODE
	switch(message) {
	case WM_CREATE:
		hdc = GetDC(hWnd);

		GetTextMetrics(hdc, &tm);
		xCharPos = tm.tmAveCharWidth;
		xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xCharPos / 2;
		yCharPos = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hWnd, hdc);
		//Set to width of 48 lowercase characters + width of 12 uppercase characters
		xClientPosMax = 48 * xCharPos + 12 * xUpper;

		return 0;

	case WM_SIZE:
		xClientPos = LOWORD(lParam);			//((WORD)(((DWORD_PTR)(l)) & 0xffff))
		yClientPos = HIWORD(lParam);			//((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

		//Set the vertical scrollbar range and page size(setting the page size will determine the thickness of the scrollbar)
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = LINES - 1;
		si.nPage = yClientPos / yCharPos;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + xClientPosMax / xCharPos;
		si.nPage = xClientPos / xCharPos;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		return 0;

	// Get all the information of the horizontal scroll bar
	case WM_HSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_HORZ, &si);


		// Save the current scrollbar position and compare later
		xScrollPos = si.nPos;
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			si.nPos -= 1;
			break;
		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}

		// Set the new position of the scrollbar slider
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

		// Get the position of the scrollbar slider, which may not be the same value due to window adjustment
		GetScrollInfo(hWnd, SB_HORZ, &si);

		// Compare with the previously saved values, if different, scroll the window
		if (si.nPos != xScrollPos) {
			ScrollWindow(hWnd, xCharPos *(xScrollPos - si.nPos), 0, NULL, NULL);
		}

		return 0;

	case WM_VSCROLL:
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		yScrollPos = si.nPos;
		switch (LOWORD(wParam)) {
		//HOME button
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		//END button
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}

		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		if (si.nPos != yScrollPos) {
			ScrollWindow(hWnd, 0, yCharPos * (yScrollPos - si.nPos), NULL, NULL);
			UpdateWindow(hWnd);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Get the position of the vertical scroll bar
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd, SB_VERT, &si);
		yScrollPos = si.nPos;

		GetScrollInfo(hWnd, SB_HORZ, &si);
		xScrollPos = si.nPos;

		// Calculate the area that needs to be redrawn
		faFirstLine = max(0, yScrollPos + ps.rcPaint.top / yCharPos);
		faLastLine = min(LINES - 1, yScrollPos + ps.rcPaint.bottom / yCharPos);

		for (int loopCounter = faFirstLine; loopCounter <= faLastLine; loopCounter++) {
			xStringPos = xCharPos * (1 - xScrollPos);
			yStringPos = yCharPos * (loopCounter - yScrollPos);

			hr = StringCchLength(szString[loopCounter], 4096, &strLength);

			// Not understand
			/*if (FAILED(hr) | strLength == NULL) {
				MessageBox(hWnd, TEXT("Failed to get string length!"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				return 0;
			}*/

			// draw a row of data in the client area
			TextOut(hdc, xStringPos, yStringPos, szString[loopCounter], strLength);
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
