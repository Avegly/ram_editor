//avegly ram editor version 0.1
#include <Windows.h>
#include <sstream>
#include <string>
#include <cstdio>
#include "resource.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")


HWND hEdit1;
HWND hEdit2;
HWND hEdit3;

#define BTN_SHOW   1
#define BTN_HELLO  2

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


void writeValue()
{
    char windowTitle[256];
    char inputAddress[256];
    char inputText[256];

    GetWindowText(hEdit1, windowTitle, sizeof(windowTitle));
    GetWindowText(hEdit2, inputAddress, sizeof(inputAddress));
    GetWindowText(hEdit3, inputText, sizeof(inputText));

    char* endPtr;
    int inputValue1 = strtol(inputText, &endPtr, 10);

    if (endPtr == inputText)
    {
        MessageBox(NULL, "Invalid integer", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwnd = FindWindow(NULL, windowTitle);
	if (hwnd == NULL)
	{
        MessageBox(NULL, "Window not found", "Error", MB_OK | MB_ICONERROR);
	}
    	else
	{
		DWORD procID;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

		if (procID == NULL)
		{
			MessageBox(NULL, "Window not found", "Error", MB_OK | MB_ICONERROR);
		}

		else
		{								
            char* endPtr;
            uintptr_t addr = strtoull(inputAddress, &endPtr, 16);

            if (endPtr == inputAddress)
            {
               MessageBox(NULL, "Invalid address format", "Error", MB_OK);
               return;
            }

            LPVOID ptr = reinterpret_cast<LPVOID>(addr);
            WriteProcessMemory(handle, ptr, &inputValue1, sizeof(inputValue1), 0);
            char buffer[128];

            snprintf(buffer, sizeof(buffer), "New Value: %d", inputValue1);

            MessageBox(NULL, buffer, "Message", MB_OK | MB_ICONINFORMATION);
		}
	}

}


void ReadAddress()
{
    char windowTitle[256];
    char inputAddress[256];

    GetWindowText(hEdit1, windowTitle, sizeof(windowTitle));
    GetWindowText(hEdit2, inputAddress, sizeof(inputAddress));

	int readValue = 0;

	HWND hwnd = FindWindow(NULL, windowTitle);
	if (hwnd == NULL)
	{
        MessageBox(NULL, "Window not found", "Error", MB_OK | MB_ICONERROR);
	}

	else
	{
		DWORD procID;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

		if (procID = NULL)
		{
            MessageBox(NULL, "Cannot obtain process.", "Error", MB_OK | MB_ICONERROR);
		}

		else
		{									  

            char* endPtr;
            uintptr_t addr = strtoull(inputAddress, &endPtr, 16);

            if (endPtr == inputAddress)
            {
               MessageBox(NULL, "Invalid address format", "Error", MB_OK);
               return;
            }

            PBYTE* ptr = reinterpret_cast<PBYTE*>(addr);
            ReadProcessMemory(handle, ptr, &readValue, sizeof(readValue), 0);					 
			
            char buffer[128];

            snprintf(buffer, sizeof(buffer), "Value:  %d", readValue);

            MessageBox(NULL, buffer, "Message", MB_OK | MB_ICONINFORMATION);
		}
	}
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case BTN_SHOW:
            writeValue();
            return 0;

        case BTN_HELLO:
            ReadAddress();
            return 0;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC  = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    WNDCLASSEX wc = {};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "window";
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wc.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
        "window",
        "Avegly Ram Editor",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        100, 100, 380, 200,
        NULL, NULL, hInstance, NULL
    );


    CreateWindow("STATIC", "Window Title:", WS_CHILD | WS_VISIBLE,20, 22, 100, 20, hwnd, NULL, hInstance, NULL);
        

    CreateWindow("STATIC", "Real Address:", WS_CHILD | WS_VISIBLE,20, 52, 100, 20, hwnd, NULL, hInstance, NULL);
        

    CreateWindow("STATIC", "New Value:", WS_CHILD | WS_VISIBLE,20, 82, 100, 20, hwnd, NULL, hInstance, NULL);
        

    hEdit1 = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,120, 20, 240, 20, hwnd, NULL, hInstance, NULL);
        

    hEdit2 = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,120, 50, 240, 20, hwnd, NULL, hInstance, NULL);
        

    hEdit3 = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,120, 80, 240, 20, hwnd, NULL, hInstance, NULL);
        

    CreateWindow("BUTTON", "Write",WS_CHILD | WS_VISIBLE,40, 130, 130, 30,hwnd, (HMENU)BTN_SHOW, hInstance, NULL);
        
    
    CreateWindow("BUTTON", "Read",WS_CHILD | WS_VISIBLE,200, 130, 130, 30,hwnd, (HMENU)BTN_HELLO, hInstance, NULL);
        
        
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
