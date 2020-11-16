#include "tictactoe32.h"
#include "tictactoe32UI.h"
#include "resource.h"

extern GAME* g_tictactoe;
TCHAR g_symbol[2][2] = {{TEXT("X")}, {TEXT("O")}};

BOOL CALLBACK DialogProc(
	_In_ HWND hwnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	switch (uMsg) {
		case WM_INITDIALOG: {
			return true;
		}
		case WM_CLOSE: {
			EndDialog(hwnd, 0);
			return true;
		}
		case WM_COMMAND: {
			int result = 0;
			int idc = LOWORD(wParam);
			switch (idc) {
				case IDC_BUTTON_A0: {
					result = g_tictactoe->set_map(MAP_A0);
					break;
				}
				case IDC_BUTTON_A1: {
					result = g_tictactoe->set_map(MAP_A1);
					break;
				}
				case IDC_BUTTON_A2: {
					result = g_tictactoe->set_map(MAP_A2);
					break;
				}
				case IDC_BUTTON_B0: {
					result = g_tictactoe->set_map(MAP_B0);
					break;
				}
				case IDC_BUTTON_B1: {
					result = g_tictactoe->set_map(MAP_B1);
					break;
				}
				case IDC_BUTTON_B2: {
					result = g_tictactoe->set_map(MAP_B2);
					break;
				}
				case IDC_BUTTON_C0: {
					result = g_tictactoe->set_map(MAP_C0);
					break;
				}
				case IDC_BUTTON_C1: {
					result = g_tictactoe->set_map(MAP_C1);
					break;
				}
				case IDC_BUTTON_C2: {
					result = g_tictactoe->set_map(MAP_C2);
					break;
				}
				case IDC_BUTTON_RESET: {
					g_tictactoe->restart();
					return true;
				}
				case IDC_BUTTON_CLEAR: {
					g_tictactoe->reset();
					return true;
				}
			}

			HWND hwndButton = GetDlgItem(hwnd, idc);
			if (idc >= 1000 && idc <= 1008) {
				int turn = g_tictactoe->turn();
				SetWindowText(hwndButton, (LPCWSTR)&g_symbol[turn]);
				EnableWindow(hwndButton, false);
			}

			switch (result) {
			case DRAW:
				MessageBox(hwnd, TEXT("DRAW"), TEXT("Game Over"), MB_OK);
				break;
			case P0_WIN:
				MessageBox(hwnd, TEXT("Player_0 WIN"), TEXT("Game Over"), MB_OK);
				break;
			case P1_WIN:
				MessageBox(hwnd, TEXT("Player_1 WIN"), TEXT("Game Over"), MB_OK);
				break;
			}
		}
	}
	return false;
}

