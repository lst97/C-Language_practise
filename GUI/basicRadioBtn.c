/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicRadioBtn.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 13/04/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Create five radio button give user to select one option.
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 13042019    lst97       1      First release
;*
;* Known Issue       :
;* 1. Not truly random
;*
;|**********************************************************************;
*/

#include <windows.h>
#include <stdlib.h>
#include <time.h>

//DEFINE
#define NUMOFBUTTON 5
#define ID_GROUPBUTTON_00 (NUMOFBUTTON + 1)
#define ID_PUSHBUTTON_00 (NUMOFBUTTON + 2)
#define ID_PUSHBUTTON_01 (NUMOFBUTTON + 3)
#define ID_PUSHBUTTON_02 (NUMOFBUTTON + 4)
#define ID_PUSHBUTTON_03 (NUMOFBUTTON + 5)

//DECLAR
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//DATA
static char szAppName[] = "Basic GUI - Radio Button";
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
	wndClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			//A handle to the class background brush.
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
	RECT rect;
	TCHAR *option[NUMOFBUTTON] = {TEXT("OPTION 1"), TEXT("OPTION 2"), TEXT("OPTION 3"), TEXT("OPTION 4"), TEXT("OPTION 5") };	// Create array so that messagebox can use it
	static int xCharPos, yCharPos;
	static HWND hwndButton[NUMOFBUTTON], hwndChild, hwndBtnAuto, hwndBtnSpecial, hwndBtnHidden, hwndBtnGroupBox;		// All button handle
	int id;
	static int selectedRadio = -1;

	//CODE
	switch (message) {
	case WM_CREATE:
		xCharPos = LOWORD(GetDialogBaseUnits());		// Position Reference X
		yCharPos = HIWORD(GetDialogBaseUnits());		// Position Reference Y

		GetWindowRect(hWnd, &rect);
		MoveWindow(hWnd, rect.left, rect.top, 28 * xCharPos, 14 * yCharPos, TRUE);

		for (int loopCounter = 0; loopCounter < NUMOFBUTTON -1; loopCounter++) {		// Create 4 radio options
			hwndButton[loopCounter] = CreateWindow(TEXT("button"), option[loopCounter], BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE, 2 * xCharPos, yCharPos * (2 + 2 * loopCounter), 11 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)loopCounter, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		}

		// Hidden radio option 5
		hwndButton[NUMOFBUTTON -1] = CreateWindow(TEXT("button"), option[4], BS_AUTORADIOBUTTON | WS_CHILD, 2 * xCharPos, yCharPos * 2 * NUMOFBUTTON, 11 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)(NUMOFBUTTON -1), ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		// Other 4 normal buttons
		hwndBtnGroupBox = CreateWindow(TEXT("button"), TEXT("Group"), BS_GROUPBOX | WS_CHILD | WS_VISIBLE, xCharPos, yCharPos / 2, 13 * xCharPos, 10 * yCharPos, hWnd, (HMENU)ID_GROUPBUTTON_00, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		CreateWindow(TEXT("button"), TEXT("SUBMITE!"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 15 * xCharPos, yCharPos, 10 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)ID_PUSHBUTTON_00, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndBtnAuto = CreateWindow(TEXT("button"), TEXT("AUTO!"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 15 * xCharPos, 3 * yCharPos, 10 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)ID_PUSHBUTTON_01, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndBtnSpecial = CreateWindow(TEXT("button"), TEXT("*HIDDEN"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE | WS_DISABLED, 15 * xCharPos, 5 * yCharPos, 10 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)ID_PUSHBUTTON_02, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndBtnHidden = CreateWindow(TEXT("button"), TEXT("INVISIBLE"), BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 15 * xCharPos, 7 * yCharPos, 10 * xCharPos, 7 * yCharPos / 4, hWnd, (HMENU)ID_PUSHBUTTON_03, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		return 0;

	case WM_COMMAND:
		id = LOWORD(wParam);			// LOWORD(wParam) Stored the id of the button.
		if (id < NUMOFBUTTON) {			// Have to match hwndButton[] array size.
			selectedRadio = id;
		}

		switch (id){
		case ID_PUSHBUTTON_00:			// Submite
			if (selectedRadio == -1) {
				MessageBox(hWnd, TEXT("NOTHING SELECTED!"), TEXT("MESSAGE"), MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBox(hWnd, option[selectedRadio], TEXT("AUTO"), MB_OK | MB_ICONINFORMATION);
			}
			break;

		case ID_PUSHBUTTON_01:			// AUTO!
			hwndChild = GetDlgItem(hWnd, selectedRadio);
			SendMessage(hwndChild, BM_SETCHECK, BST_UNCHECKED, NULL);

			srand((unsigned int)time(NULL));
			selectedRadio = rand() % NUMOFBUTTON -1;

			hwndChild = GetDlgItem(hWnd, selectedRadio);
			SendMessage(hwndChild, BM_SETCHECK, BST_CHECKED, NULL);

			EnableWindow(hwndBtnAuto, FALSE);

			for (int loopCounter = 0; loopCounter < NUMOFBUTTON; loopCounter++) {
				EnableWindow(hwndButton[loopCounter], FALSE);
			}
			break;

		case ID_PUSHBUTTON_02:			// *Hidden!
			GetWindowRect(hWnd, &rect);
			MoveWindow(hWnd, rect.left, rect.top, 28 * xCharPos, 16 * yCharPos, TRUE);

			MoveWindow(hwndBtnGroupBox, xCharPos, yCharPos / 2, 13 * xCharPos, 12 * yCharPos, TRUE);

			ShowWindow(hwndButton[NUMOFBUTTON -1], SW_SHOWNORMAL);
			break;

		case ID_PUSHBUTTON_03:			// hiddent btn
			EnableWindow(hwndBtnSpecial, TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
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
