/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : basicButton.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 16/11/2020
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : Basic Tictactoe
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 16112020    lst97       1      First release
;*
;* Known Issue       :
;* Game info panel, reset, clear button now yet implement
;*
;|**********************************************************************;
*/

#include "tictactoe32.h"
#include "tictactoe32UI.h"
#include "resource.h"
#include <Windows.h>

GAME* g_tictactoe;
int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int cCmdShow
) {
	g_tictactoe = new GAME();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProc);

	return 0;
}
