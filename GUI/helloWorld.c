/*
;|**********************************************************************;
;* Project           : Basic_C_GUI
;*
;* Program name      : helloWorld.c
;*
;* Author            : SIO TOU LAI (laisiotou1997@gmail.com)
;*
;* Date created      : 24/02/2019
;*
;* Version           : Internal
;*
;* Copyright         : GNU GENERAL PUBLIC LICENSE Version 3
;*
;* Purpose           : messageBox using C Language
;*
;* Revision History  :
;*
;* Date        Author      Ref    Revision (Date in DDMMYYYY format)
;* 24022019    lst97       1      First release
;*
;* Known Issue       :
;*
;|**********************************************************************;
*/

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	static char szContent[] = "Hello World";
	static char szCaption[] = "Message";

	//Fix for unicode problem.
	//static TCHAR szContent[] = TEXT("Hello World"); //TCHAR == wchar_t
	//static TCHAR szCaption[] = TEXT("Message");

	MessageBox(NULL, szContent, szCaption, MB_OK);
	return 0;
}
