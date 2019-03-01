/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : textMetrics.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 01/03/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Create a list using TextMetrics
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 0103022019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>
#include "strsafe.h"

//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA:
static char szAppName[] = "Form1";
static char szErrorMessage[] = "This program only run on Windows NT!";
static int nWidth = 640;
static int nHeight = 480;
static int cxChar, cyChar;

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

	HWND hWnd;
	//If the function succeeds, the return value is a handle to the new window, NULL if fails.
	hWnd = CreateWindow(szAppName, szAppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, hInstance, NULL);

	//nCmdShow	控制窗口如何显示，如果发送应用程序的程序提供了 STARTUPINFO 结构，则应用程序第一次调用 ShowWindow 时该参数被忽略。否则，在第一次调用 ShowWindow 函数时，该值应为在函数 WinMain 中 nCmdShow 参数。
	//1. 如果窗口之前可见，则返回值为非 0；
	//2. 如果窗口之前被隐藏，则返回值为 0。
	ShowWindow(hWnd, nCmdShow);

	//UpdateWindow 函数绕过应用程序的消息队列，直接发送 WM_PAINT 消息给指定窗口的窗口过程。
	UpdateWindow(hWnd);

	/*
	typedef struct tagMSG {
	  HWND   hWnd;
	  UINT   message;
	  WPARAM wParam;
	  LPARAM lParam;
	  DWORD  time;
	  POINT  pt;
	} MSG, *PMSG, *LPMSG;
	*/
	MSG msg;
	/*
	BOOL GetMessage(
	  LPMSG lpMsg,
	  HWND  hWnd,
	  UINT  wMsgFilterMin,
	  UINT  wMsgFilterMax
	);

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
	/*
	typedef struct tagPAINTSTRUCT {
	  HDC  hdc;
	  BOOL fErase;
	  RECT rcPaint;
	  BOOL fRestore;
	  BOOL fIncUpdate;
	  BYTE rgbReserved[32];
	} PAINTSTRUCT, *PPAINTSTRUCT;
	*/
	PAINTSTRUCT ps;

	//RECT 结构定义了一个矩形的左上角和右下角的坐标。
	/*
	typedef struct _RECT {
	  LONG left;
	  LONG top;
	  LONG right;
	  LONG bottom;
	} RECT, *PRECT;
	*/
	RECT rect;

	//DATA
	char szBuffer[32];
	size_t strLength;
	TEXTMETRIC tm;

	//CODE
	switch (message) {
	case WM_CREATE:
		hdc = GetDC(hWnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hWnd, hdc);

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		GetClientRect(hWnd, &rect);
		for (unsigned short int row = 0; row < 10; row++) {
			StringCchPrintf(szBuffer, 32, TEXT("%d. %s"), row + 1, TEXT("laisiotou1997@gmail.com"));
			StringCchLength(szBuffer, 32, &strLength);
			TextOut(hdc, cxChar, (row + 1) * cyChar, szBuffer, strLength);
		}

		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Are you sure you want to Quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)) {
		case IDYES:
			DestroyWindow(hWnd);
		default:
			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
