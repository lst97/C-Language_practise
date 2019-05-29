/*
;|**********************************************************************;
;* Project           : 100 Classic Examples (C Language)
;*
;* Program name      : Basic_C_100Q.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 29/05/2019
;*
;* Version           : v0.1.0 (beta 01 - build 01)
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Showing different solutions for 100 Classic C  Examples
;*
;* Revision History  :
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 29052019    lst97       1      First release
;*
;* Known Issue       :
;* - Useless textbox.
;* - Old school elements
;* - The white area under the textbox field.
;*
;* TODO              :
;* + Kill the C process which been opened before. (next build 02)
;* + Ctrl + A to select all text in the textbox. (next build 02)
;* + Export all the source code in one click. (next build 02)
;* + Try to integrate all 101 C programs into the Windows program. (Still learning WinAPI, don't know how to it yet)
;* + Debug command and direct input to the C process using the textbox.
;* + Translation.
;* & Any suggestion?
;*
;|**********************************************************************;
*/
// Code review: N/A

#include "resource.h"
#include <windows.h>
#include <math.h>	// for fnIntToString(), fnIntToStringA()

//DEFINE
#define MAXCONTENTBUFFER 4096

#define MAXABOUTBUFFER 512
#define MINABOUTBUFFER 64

#define TXTBOXBUFFER 64

#define STRCOMBOBOXBUFFER 128

#define DIRECTORYBUFFER 128

#define RECTMIDPOS (rect.right / 2)
#define XLEFTMARGIN (xRefPos *1)
#define XRIGHTMARGIN (rect.right - (xRefPos *2))

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND , UINT , WPARAM , LPARAM );

void comboBox();
void btnRun(HWND);
void btnCopy(HWND);
void btnExport();

void fnIntToString(TCHAR [4], unsigned short);
void fnIntToStringA(char [4], unsigned short);

//DATA
static TCHAR AppName[] = TEXT("100 Classic Examples (C Language) v0.1.0_beta");
static TCHAR OsError[] = TEXT("This program only runs on Windows NT or abrove");

int ClientWidth = 640;
int ClientHeight = 480;

typedef struct {
	LONG comboBox;
	LONG txtContent;
	LONG txtInput;
	LONG lableInput;
	LONG btnCopy;
	LONG btnRun;
	LONG BtnExport;
}Elements_Style;

Elements_Style style = {
	(CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL) ,
	(ES_MULTILINE | ES_READONLY | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL),
	(WS_CHILD | WS_VISIBLE | WS_DISABLED),
	(WS_CHILD | WS_VISIBLE),
	(BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_DISABLED),
	(BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_DISABLED),
	(BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_DISABLED)
};

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
	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			//A handle to the class background brush.
	wndClass.lpszMenuName = (LPCWSTR)IDR_MENU;								//The resource name of the class menu, as the name appears in the resource file.
	wndClass.lpszClassName = AppName;								//The maximum length for lpszClassName is 256.

	if (!RegisterClass(&wndClass)) {								//Value is zero if Register FAIL (!0) = TRUE;
		MessageBox(NULL, OsError, AppName, MB_ICONERROR);
		return 0;
	}

	HWND hwnd;
	hwnd = CreateWindow(AppName, AppName, WS_TILED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, ClientWidth, ClientHeight, NULL, NULL, hInstance, NULL);
	
	ShowWindow(hwnd, SW_SHOWNORMAL);		// nCmdShow
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

TCHAR szTxtContentBuffer[MAXCONTENTBUFFER];

static HINSTANCE hInstanceResource;
static HWND hwndComboBox;
static HWND hwndTxtContent, hwndTxtInput;
static HWND hwndLableTitle;		// no need lable input
static HWND hwndBtnCopy, hwndBtnRun, hwndBtnExport;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//DATA
	static RECT rect;
	static int xRefPos, yRefPos;

	TCHAR szComboBoxBuffer[STRCOMBOBOXBUFFER];

	//CODE
	switch (message) {
	case WM_CREATE:
		hInstanceResource = ((LPCREATESTRUCT)lParam)->hInstance;	// Rescourse
		GetClientRect(hwnd, &rect);

		xRefPos = LOWORD(GetDialogBaseUnits());
		yRefPos = HIWORD(GetDialogBaseUnits());

		// Create elements
		hwndComboBox = CreateWindow(TEXT("COMBOBOX"), TEXT(""), style.comboBox, XLEFTMARGIN, yRefPos * 2, XRIGHTMARGIN, yRefPos * 12, hwnd, (HMENU)MAIN_COMBOBOX, (HINSTANCE)lParam, NULL);
		for (unsigned short iLoopCounter = IDS_STRING_COMBOBOX001; iLoopCounter <= IDS_STRING_COMBOBOX101; iLoopCounter++) {
			LoadString(hInstanceResource, iLoopCounter, szComboBoxBuffer, STRCOMBOBOXBUFFER);
			SendMessage(hwndComboBox, CB_ADDSTRING, wParam, (LPARAM)szComboBoxBuffer);
		}

		hwndTxtContent = CreateWindow(TEXT("EDIT"), TEXT("- Please select a question\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n"), style.txtContent, XLEFTMARGIN, yRefPos * 4, XRIGHTMARGIN, yRefPos * 16, hwnd, (HMENU)MAIN_TXTCONTENT, (HINSTANCE)lParam, NULL);
		CreateWindow(TEXT("STATIC"), TEXT("Input:"), style.lableInput, XLEFTMARGIN, yRefPos * 21, xRefPos *10, yRefPos * 22, hwnd, 0, (HINSTANCE)lParam, NULL);
		hwndTxtInput = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""), style.txtInput, XLEFTMARGIN, yRefPos * 22, XRIGHTMARGIN, yRefPos +8, hwnd, (HMENU)MAIN_TXTINPUT, (HINSTANCE)lParam, NULL);
		hwndBtnCopy = CreateWindow(TEXT("BUTTON"), TEXT("Copy"), style.btnCopy, XLEFTMARGIN, yRefPos * 24, xRefPos *10, yRefPos * 2, hwnd, (HMENU)MAIN_IDCOPY, (HINSTANCE)lParam, NULL);
		hwndBtnRun = CreateWindow(TEXT("BUTTON"), TEXT("RUN"), style.btnRun, RECTMIDPOS - (xRefPos * 5), yRefPos * 24, RECTMIDPOS - (xRefPos * 29), yRefPos * 2, hwnd, (HMENU)MAIN_IDRUN, (HINSTANCE)lParam, NULL);
		hwndBtnExport = CreateWindow(TEXT("BUTTON"), TEXT("Export"), style.btnCopy, XRIGHTMARGIN - (xRefPos * 9), yRefPos * 24, XRIGHTMARGIN - (xRefPos *66), yRefPos * 2, hwnd, (HMENU)MAIN_IDEXPORT, (HINSTANCE)lParam, NULL);
		hwndLableTitle = CreateWindow(TEXT("STATIC"), TEXT("Question Selector:"), style.lableInput, XLEFTMARGIN, yRefPos, XRIGHTMARGIN, yRefPos, hwnd, 0, (HINSTANCE)lParam, NULL);
	
	case WM_COMMAND:
		if (HIWORD(wParam) == CBN_SELCHANGE) comboBox();
		else if (!lParam) {	// For the menu message
			switch (LOWORD(wParam)) {
			case ID_PROGRAM_EXIT:
				SendMessage(hwnd, WM_CLOSE, wParam, lParam);
				break;
			case ID_HELP_ABOUT:
				DialogBox(hInstanceResource, (LPCWSTR)IDD_DIALOG_ABOUT, hwnd, AboutDlgProc);
				break;
			}
		}else{	// For DC message
			switch (LOWORD(wParam)) {
			case MAIN_IDRUN:
				btnRun(hwnd);
				break;
			case MAIN_IDCOPY:
				btnCopy(hwnd);
				break;
			case MAIN_IDEXPORT:;
				btnExport();
				break;
			}
		}
		break;

	case WM_CLOSE:
		switch (MessageBox(hwnd, TEXT("Do you really want to quite?"), TEXT("Message"), MB_YESNO | MB_ICONQUESTION)) {
		case IDYES:
			PostQuitMessage(0);
			break;
		case IDNO: break;
		}
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL CALLBACK AboutDlgProc(HWND hAboutDlgProc, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ABOUT_IDDETAIL:;
			static LPWSTR szAboutDetail[MAXABOUTBUFFER];
			LoadString(hInstanceResource, IDS_STRING_ABOUTDETAIL, (LPWSTR)szAboutDetail, MAXABOUTBUFFER);
			MessageBox(hAboutDlgProc, (LPWSTR)szAboutDetail, TEXT("About - Detail"), MB_OK);
			break;
		case IDCANCEL:
		case ABOUT_IDOK:
			EndDialog(hAboutDlgProc, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void comboBox() {
	// For debug command and direct input to the C process, function not yet been added (beta 01 build 01) (lst97 29052019)
	TCHAR szBuffer[TXTBOXBUFFER] = TEXT("This Question ["), szNumArray[4], szBufferEnd[] = TEXT("] input not yet support");
	int iCaseNum = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);

	LoadString(hInstanceResource, iCaseNum + IDS_STRING_CONTENT001, szTxtContentBuffer, MAXCONTENTBUFFER);

	fnIntToString(szNumArray, iCaseNum + 1);
	lstrcat(szBuffer, szNumArray);
	lstrcat(szBuffer, szBufferEnd);
	SetWindowText(hwndTxtInput, szBuffer);

	// Handle 2 special cases that can't be export or copy.
	if (iCaseNum == 94 || iCaseNum == 100) {
		SetWindowText(hwndTxtContent, (LPCWSTR)szTxtContentBuffer);
		EnableWindow(hwndBtnRun, TRUE);
		EnableWindow(hwndBtnCopy, FALSE);
		EnableWindow(hwndBtnExport, FALSE);
		EnableWindow(hwndTxtInput, FALSE);
	} else {
		SetWindowText(hwndTxtContent, (LPCWSTR)szTxtContentBuffer);
		EnableWindow(hwndBtnRun, TRUE);
		EnableWindow(hwndBtnCopy, TRUE);
		EnableWindow(hwndBtnExport, TRUE);
		EnableWindow(hwndTxtInput, FALSE);
	}
}

void btnRun(HWND hwnd) {
	TCHAR szDirectoryBuffer[DIRECTORYBUFFER];
	TCHAR szBuffer[32] = TEXT("\\C_100Q\\C_100Q_"), szNumArray[4], szBufferEnd[] = TEXT(".exe");
	TCHAR szNoFoundBuffer[64] = TEXT("The specified file [C_100Q_"), szNoFoundBufferEND[] = TEXT(".exe] not found or invalid!");
	TCHAR szRunningBuffer[32] = TEXT("[C_100Q_"), szRunningBufferEND[] = TEXT(".exe] is - RUNNING");
	int iCaseNum = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
	char szExeBuffer_ASCII[32] = ".\\C_100Q\\C_100Q_", szNumArray_ASCII[4], szExeBufferEnd_ASCII[] = ".exe";

	GetCurrentDirectory(DIRECTORYBUFFER, szDirectoryBuffer);
	fnIntToString(szNumArray, iCaseNum + 1);
	lstrcat(szDirectoryBuffer, szBuffer);
	lstrcat(szDirectoryBuffer, szNumArray);
	lstrcat(szDirectoryBuffer, szBufferEnd);

	fnIntToStringA(szNumArray_ASCII, iCaseNum + 1);
	lstrcatA(szExeBuffer_ASCII, szNumArray_ASCII);
	lstrcatA(szExeBuffer_ASCII, szExeBufferEnd_ASCII);

	lstrcat(szNoFoundBuffer, szNumArray);
	lstrcat(szNoFoundBuffer, szNoFoundBufferEND);

	lstrcat(szRunningBuffer, szNumArray);
	lstrcat(szRunningBuffer, szRunningBufferEND);
	if (!FindWindow(NULL, szDirectoryBuffer)) {
		// For killing the C process opened by this program. Planed to fix in the next build (beta 02, build 02) (lst97 29052019)
		// ShellExecute(NULL, L"open", L"cmd.exe", L"/C taskkill /IM C_100Q_002.exe", NULL, SW_HIDE);		// Looking for alternative or better algorithm (lst97)

		// If the function succeeds, return value is greater than 31.
		if (WinExec(szExeBuffer_ASCII, SW_SHOWNORMAL) <= 31) MessageBox(hwnd, szNoFoundBuffer, TEXT("ERROR"), MB_OK | MB_ICONERROR);
	}
	else MessageBox(hwnd, szRunningBuffer, TEXT("Message"), MB_OK | MB_ICONINFORMATION);
}

void btnCopy(HWND hwnd) {
	GetWindowText(hwndTxtContent, szTxtContentBuffer, MAXCONTENTBUFFER);
	HGLOBAL hSharedMem = GlobalAlloc(GHND | GMEM_SHARE, lstrlen((szTxtContentBuffer)+1) * sizeof(TCHAR));
	HGLOBAL pSharedMem = GlobalLock(hSharedMem);	// Point to hSharedMem
	lstrcpy(pSharedMem, szTxtContentBuffer);
	GlobalUnlock(hSharedMem);
	OpenClipboard(hwnd);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hSharedMem);
	CloseClipboard();
	GlobalFree(hSharedMem);
};

void btnExport() {
	HANDLE hFile = NULL;
	TCHAR szBuffer[32] = TEXT(".\\Source\\C_100Q_"), szNumArray[4], szBufferEnd[] = TEXT(".c");
	int iCaseNum = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);

	fnIntToString(szNumArray, iCaseNum + 1);
	lstrcat(szBuffer, szNumArray);
	lstrcat(szBuffer, szBufferEnd);

	CreateDirectory(TEXT(".\\Source"), NULL);

	hFile = CreateFile(szBuffer, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	GetWindowText(hwndTxtContent, szTxtContentBuffer, MAXCONTENTBUFFER);
	WriteFile(hFile, szTxtContentBuffer, lstrlen((szTxtContentBuffer)+1) * sizeof(TCHAR), 0, 0);
	CloseHandle(hFile);
	WinExec("explorer.exe .\\Source", SW_SHOWNORMAL);
}

void fnIntToString(TCHAR SZArray[4], unsigned short INum) {
	for (unsigned short iLoop = 0; iLoop < 3; iLoop++) {
		SZArray[-(iLoop - 2)] = (TCHAR)((INum % 10) + 0x30);
		INum /= 10;
	}
	SZArray[3] = '\0';
}

void fnIntToStringA(char SZArray[4], unsigned short INum) {
	for (unsigned short iLoop = 0; iLoop < 3; iLoop++) {
		SZArray[-(iLoop - 2)] = (char)((INum % 10) + 0x30);
		INum /= 10;
	}
	SZArray[3] = '\0';
}
