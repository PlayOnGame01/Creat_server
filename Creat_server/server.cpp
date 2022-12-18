#include <ws2tcpip.h> 
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include "resource.h"
#include "WinSock2.h" 
#pragma comment(lib, "Ws2_32.lib") 

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hStart, hEnd, hSend, hText, hReceive;
WCHAR str_text[50];
const int MAXSTRLEN = 255;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET _socket;
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKET acceptSocket;
	acceptSocket = accept(_socket, NULL, NULL);

	switch (message)
	{
	case WM_INITDIALOG: {
		hStart = GetDlgItem(hWnd, IDC_START);
		hEnd = GetDlgItem(hWnd, IDC_END);
		hSend = GetDlgItem(hWnd, IDC_SEND);
		hText = GetDlgItem(hWnd, IDC_EDIT1);
		hReceive = GetDlgItem(hWnd, IDC_EDIT2);
	}
					  break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_START) {
			Button_Enable(hSend, TRUE);

			WSADATA wsaData;
			sockaddr_in addr;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
			addr.sin_family = AF_INET;
			inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
			addr.sin_port = htons(20000);
			bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
			listen(_socket, 1);
			MessageBox(hWnd, TEXT("Connect sucssesful"), TEXT("Conect"), MB_OK);

			char buf[MAXSTRLEN];
			int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
			SendMessage(hReceive, WM_SETTEXT, 0, LPARAM(buf));

		}

		if (LOWORD(wParam) == IDC_SEND) {
			int size = GetWindowTextLength(hText);
			GetDlgItemText(hWnd, IDC_EDIT1, str_text, size + 1);

			char text[50];
			WideCharToMultiByte(CP_UTF8, NULL, str_text, -1, text, 0, NULL, NULL);

			send(acceptSocket, text, strlen(text), 0);

			closesocket(acceptSocket);
			closesocket(_socket);
			WSACleanup();
		}

	}
				   break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}