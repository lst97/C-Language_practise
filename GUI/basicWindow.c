/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      :basicWindow.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 25/02/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : basic window
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 25022019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>

//Declar
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA:
static char szAppName[] = "Form1";
static char szErrorMessage[] = "This program only run on Windows NT!";
static char szPaintMessage[] = "This is the first Windows 32-bit program!";
static int nWidth = 640;
static int nHeight = 480;

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

	- - - - - [dwStyle] - - - - -
	WS_BORDER				创建一个带边框的窗口
			
	WS_CAPTION				创建一个有标题框的窗口（包含了 WS_BODER 风格）

	WS_CHILD				创建一个子窗口，这个风格的窗口不能拥有菜单也不能与 WS_POPUP 风格合用

	WS_CHILDWINDOW			与 WS_CHILD 相同

	WS_CLIPCHILDREN			当在父窗口内绘图时，排除子窗口区域，在创建父窗口时使用这个风格

	WS_CLIPSIBLINGS			1. 排除子窗口之间的相对区域，也就是，当一个特定的窗口接收到 WM_PAINT 消息时，WS_CLIPSIBLINGS 风格将所有层叠窗口排除在绘图之外，只重绘指定的子窗口

							2. 如果未指定该风格，并且子窗口是层叠的，则在重绘子窗口的客户区时，就会重绘邻近的子窗口

	WS_DISABLED				1. 创建一个初始状态为禁止的子窗口，一个禁止状态的窗口不能接受来自用户的输入信息

							2. 在窗口创建之后，可以调用 EnableWindow 函数来启用该窗口

	WS_DLGFRAME				创建一个带对话框边框风格的窗口，这种风格的窗口不能带标题条

	WS_GROUP				1. 指定一组“控制窗口”的第一个“控制窗口”

							2. 这个“控制窗口”组由第一个“控制窗口”和随后定义的“控制窗口”组成，自第二个“控制窗口”开始每个“控制窗口”具有 WS_GROUP 风格

							3. 每个组的第一个“控制窗口”带有 WS_TABSTOP 风格，从而使用户可以在组间移动

							4. 用户随后可以使用光标在组内的控制间改变键盘焦点

	WS_HSCROLL				创建一个有水平滚动条的窗口

	WS_ICONIC				创建一个初始状态为最小化状态的窗口，与 WS_MINIMIZE 风格相同

	WS_MAXIMIZE				创建一个初始状态为最大化状态的窗口

	WS_MAXIMIZEBOX			创建一个具有最大化按钮的窗口，该风格不能与 WS_EX_CONTEXTHELP 风格同时出现，同时必须指定 WS_SYSMENU 风格

	WS_MINIMIZE				创建一个初始状态为最小化状态的窗口，与 WS_ICONIC 风格相同

	WS_MINIMIZEBOX			创建一个具有最小化按钮的窗口，该风格不能与 WS_EX_CONTEXTHELP 风格同时出现，同时必须指定 WS_SYSMENU 风格

	WS_OVERLAPPED			产生一个层叠的窗口，一个层叠的窗口有一个标题条和一个边框，与 WS_TILED 风格相同

	WS_OVERLAPPEDWINDOW		相当于（WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX），与 WS_TILEDWINDOW 风格相同

	WS_POPUP				创建一个弹出式窗口，该风格不能与 WS_CHILD 风格同时使用。

	WS_POPUPWINDOW			相当于（WS_POPUP | WS_BORDER | WS_SYSMENU），但 WS_CAPTION 和 WS_POPUPWINDOW 必须同时设定才能使窗口某单可见

	WS_SIZEBOX				创建一个可调边框的窗口，与 WS_THICKFRAME 风格相同

	WS_SYSMENU				创建一个在标题条上带有窗口菜单的窗口，必须同时设定 WS_CAPTION 风格

	WS_TABSTOP				1. 创建一个“控制窗口”，在用户按下 Tab 键时可以获得键盘焦点。

							2. 按下 Tab 键后使键盘焦点转移到下一具有 WS_TABSTOP 风格的“控制窗口”

	WS_THICKFRAME			创建一个具有可调边框的窗口，与 WS_SIZEBOX 风格相同

	WS_TILED				产生一个层叠的窗口，一个层叠的窗口有一个标题和一个边框，与 WS_OVERLAPPED 风格相同

	WS_TILEDWINDOW			相当于（WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX），与 WS_OVERLAPPEDWINDOW 风格相同

	WS_VISIBLE				创建一个初始状态为可见的窗口

	WS_VSCROLL				创建一个有垂直滚动条的窗口
	
	https://docs.microsoft.com/en-gb/windows/desktop/winmsg/window-styles
	https://fishc.com.cn/forum.php?mod=forumdisplay&fid=255&filter=typeid&typeid=420
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

	switch (message) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		/*
		int DrawText(
		  HDC     hdc,
		  LPCTSTR lpchText,
		  int     cchText,		//The length, in characters, of the string. If nCount is -1, then the lpchText parameter is assumed to be a pointer to a null-terminated string and DrawText computes the character count automatically.
		  LPRECT  lprc,
		  UINT    format
		);
		*/
		DrawText(hdc, szPaintMessage, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:
		switch (MessageBox(hWnd, TEXT("Are you sure you want to Quit?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2)) {
		case IDYES:
			PostQuitMessage(0);
			return 0;
		default:
			return 0;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
