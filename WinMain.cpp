#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#include <Windows.h>
#include <windowsx.h>
#include <cstdlib>
#include "Program.h"

#include "resource.h"

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

GraphicsDxD* pDxdGraphics = nullptr;
KeyboardServer keyboardServer;
MouseServer mouseServer;

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		keyboardServer.OnKeyPressed((unsigned char)wparam);
		break;
	case WM_KEYUP:
		keyboardServer.OnKeyReleased((unsigned char)wparam);
		break;
	case WM_CHAR:
		keyboardServer.OnChar((unsigned char)wparam);
		break;
	case WM_SIZE:
	{
		RECT r = {};
		GetClientRect(hWnd, &r);

		LONG cWndWidth = r.right - r.left;
		LONG cWndHeight = r.bottom - r.top;
		if (cWndWidth > 0 && cWndHeight > 0 && pDxdGraphics)
			pDxdGraphics->OnWindowResize();
	}		
		break;

	case WM_MOUSEMOVE:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		if (x > 0 && x < static_cast<int>(pDxdGraphics->GetWndWidth()) && y > 0 
			&& y < static_cast<int>(pDxdGraphics->GetWndHeight()))
		{
			mouseServer.OnMouseMove(x, y);
			if (!mouseServer.IsInWindow())
			{
				SetCapture(hWnd);
				mouseServer.OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = max(0, x);
				x = min(static_cast<int>(pDxdGraphics->GetWndWidth()) - 1, x);
				y = max(0, y);
				y = min(static_cast<int>(pDxdGraphics->GetWndHeight()) - 1, y);
				mouseServer.OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				mouseServer.OnMouseLeave();
				mouseServer.OnLeftReleased(x, y);
				mouseServer.OnRightReleased(x, y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mouseServer.OnLeftPressed(x, y);
		SetFocus(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mouseServer.OnRightPressed(x, y);
		SetFocus(hWnd);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mouseServer.OnLeftReleased(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = (short)LOWORD(lparam);
		int y = (short)HIWORD(lparam);
		mouseServer.OnRightReleased(x, y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = (short)GET_X_LPARAM(lparam);
		int y = (short)GET_Y_LPARAM(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
		{
			mouseServer.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			mouseServer.OnWheelDown(x, y);
		}		
		break;
	}
	case WM_MBUTTONDOWN:
	{
		mouseServer.OnWheelPress();
		break;
	}
	case WM_MBUTTONUP:
	{
		mouseServer.OnWheelRelease();
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	_CrtDumpMemoryLeaks();
	const wchar_t* pNomeClasse = L"dxd";
	const wchar_t* pNomeJanela = L"DXD";

	WNDCLASS wndClass = {};
	wndClass.hInstance = hInst;
	wndClass.lpfnWndProc = MsgProc;
	wndClass.lpszClassName = pNomeClasse;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));

	RegisterClass(&wndClass);

	HWND hWnd = CreateWindow(pNomeClasse, pNomeJanela, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1366, 768, NULL, NULL, hInst, NULL);
	
	pDxdGraphics = new(_aligned_malloc(sizeof(GraphicsDxD), 16))GraphicsDxD((hWnd));

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg = {};
	KeyboardClient kbdClient(keyboardServer);
	MouseClient mouseClient(mouseServer);

	Program* pProg = new(_aligned_malloc(sizeof(Program), 16))Program(kbdClient, mouseClient, pDxdGraphics);
	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		pProg->Tick();
	}

	pProg->~Program();
	_aligned_free(pProg);

	pDxdGraphics->~GraphicsDxD();
	_aligned_free(pDxdGraphics);
	
	kbdClient.FlushBuffers();
	mouseClient.FlushBuffers();
	
	DestroyWindow(hWnd);
	UnregisterClass(pNomeClasse, hInst);

	_CrtDumpMemoryLeaks();

	return 0;
}