#include <ws2tcpip.h> 
#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include "WinSock2.h" 
#pragma comment(lib, "Ws2_32.lib") 

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HWND hIP, hConnect, hDisconnect, hSend, hText, hReceive;
const int MAXSTRLEN = 255;
WCHAR str_text[50];

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}


BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET _socket;
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //������� ��������� �����, �������� TCP

	switch (message)
	{
	case WM_INITDIALOG: {
		hIP = GetDlgItem(hWnd, IDC_IP);
		hConnect = GetDlgItem(hWnd, IDC_CONNECT);
		hDisconnect = GetDlgItem(hWnd, IDC_DISCONNECT);
		hSend = GetDlgItem(hWnd, IDC_SEND);
		hText = GetDlgItem(hWnd, IDC_TEXT);
		hReceive = GetDlgItem(hWnd, IDC_RECEIVE);
	}
					  break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == IDC_CONNECT) {
			WSADATA wsaData; //��������� ��� �������� ���������� � ������������� �������
			sockaddr_in addr; //��������� ����� � ���� 

			WSAStartup(MAKEWORD(2, 2), &wsaData); //������������� �������
			addr.sin_family = AF_INET; //��������� ������� IPv4
			inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
			addr.sin_port = htons(20000);
			connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
		}

		if (LOWORD(wParam) == IDC_SEND) {
			int size = GetWindowTextLength(hText);
			GetDlgItemText(hWnd, IDC_TEXT, str_text, size + 1);

			char text[50];
			WideCharToMultiByte(CP_UTF8, NULL, str_text, -1, text, 0, NULL, NULL);

			char buf[MAXSTRLEN];
			send(_socket, text, strlen(text), 0); //���������� ��������� �� ��������� �����

			int i = recv(_socket, buf, MAXSTRLEN, 0);
			SendMessage(hReceive, WM_SETTEXT, 0, LPARAM(buf));

			closesocket(_socket); //��������� �����
			WSACleanup(); //����������� �������
		}

	}
				   break;

	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}