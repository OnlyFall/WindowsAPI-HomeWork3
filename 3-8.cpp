#include <Windows.h>
#include <iostream>
#include <random>
#include <time.h>
#define Height 1280
#define Weight 720

using namespace std;
HPEN MyPen, OldPen;
#define MAX 9
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Windows Programing KWB";



double LengthCheck(int mx, int my, int x, int y)
{
	return sqrt((mx - x)*(mx - x) + (my - y)*(my - y));
}

BOOL InCircle(int mx, int my, int x, int y, int r)
{
	if (LengthCheck(mx, my, x, y) < r)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



float Check(int x, int y, int lx, int ly, int mx)
{
	return (float)(ly - y) / (float)(lx - x)*(float)(mx - lx) + (float)ly;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, "3-8", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 100, 100, Height, Weight, NULL, (HMENU)NULL, hInstance, NULL);
	int x = 800;
	int y = 600;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
static char str;
int p = 0;
int position[100];

struct Fall {
	int x;
	int y;
};

struct fallCheck {
	int direction;
	int check;
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	HPEN myPen;
	HPEN oldPen;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	static Fall fall[10];
	static fallCheck fallCheck[10];
	static POINT poly[5];
	static int i = 0;
	static int inputX, inputY;
	static int count[10] = { 0 };

	switch (iMessage) {
	case WM_CREATE:
		
		for (int i = 0; i < 10; ++i)
		{
			fall[i].x = rand() % 800;
			fall[i].y = rand() % 200;
		}

		return 0;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		if (wParam == VK_RETURN) {
			SetTimer(hWnd, 1, 10, NULL);
			SetTimer(hWnd, 2, 500, NULL);
		}
		break;

	case WM_LBUTTONDOWN:
		inputX = LOWORD(IParam);
		inputY = HIWORD(IParam);

		if (i == 4)
			InvalidateRect(hWnd, NULL, FALSE);

		poly[i].x = inputX;
		poly[i].y = inputY;
		i++;
		break;

	case WM_TIMER:

		switch (wParam) {
		case 1:
			for (int i = 0; i < 10; ++i) {
				for (int j = 0; j < 5; ++j)
					if (InCircle(fall[i].x, fall[i].y, poly[j].x, poly[j].y, 30) && count[i] == 0) {
						fallCheck[i].check = 1;
						fallCheck[i].direction = j;
						fall[i].x = poly[j].x;
						fall[i].y = poly[j].y;
						count[i]++;
						KillTimer(hWnd, 2);
						SetTimer(hWnd, 2, 10, NULL);
					}
				
			}
			for (int i = 0; i < 10; ++i) {
				if(fallCheck[i].check == 0)
					fall[i].y += 1;
			}
			break;
		case 2:
			for (int i = 0; i < 10; ++i) {
				if (fallCheck[i].check == 1) {
					if (fallCheck[i].direction == 0)
					{
						if (poly[0].x < poly[1].x && poly[0].y < poly[1].y) {
							fall[i].x += abs(poly[0].x - poly[1].x) / 20;
							fall[i].y += abs(poly[0].y - poly[1].y) / 20;
						}
						else if (poly[0].x > poly[1].x && poly[0].y > poly[1].y) {
							fall[i].x -= abs(poly[0].x - poly[1].x) / 20;
							fall[i].y -= abs(poly[0].y - poly[1].y) / 20;
						}
						else if (poly[0].x > poly[1].x && poly[0].y < poly[1].y) {
							fall[i].x -= abs(poly[0].x - poly[1].x) / 20;
							fall[i].y += abs(poly[0].y - poly[1].y) / 20;
						}
						else if (poly[0].x < poly[1].x && poly[0].y > poly[1].y) {
							fall[i].x += abs(poly[i].x - poly[i + 1].x) / 20;
							fall[i].y -= abs(poly[i].y - poly[i + 1].y) / 20;
						}
						if (InCircle(fall[i].x, fall[i].y, poly[1].x, poly[1].y, 30) == TRUE) {
							fallCheck[i].direction = 1;
							fall[i].x = poly[1].x;
							fall[i].y = poly[1].y;
						}
					}
					else if (fallCheck[i].direction == 1)
					{
						if (poly[1].x < poly[2].x && poly[1].y < poly[2].y) {
							fall[i].x += abs(poly[1].x - poly[2].x) / 20;
							fall[i].y += abs(poly[1].y - poly[2].y) / 20;
						}
						else if (poly[1].x > poly[2].x && poly[1].y > poly[2].y) {
							fall[i].x -= abs(poly[1].x - poly[2].x) / 20;
							fall[i].y -= abs(poly[1].y - poly[2].y) / 20;
						}
						else if (poly[1].x > poly[2].x && poly[1].y < poly[2].y) {
							fall[i].x -= abs(poly[1].x - poly[2].x) / 20;
							fall[i].y += abs(poly[1].y - poly[2].y) / 20;
						}
						else if (poly[1].x < poly[2].x && poly[1].y > poly[2].y) {
							fall[i].x += abs(poly[1].x - poly[2].x) / 20;
							fall[i].y -= abs(poly[1].y - poly[2].y) / 20;
						}
						if (InCircle(fall[i].x, fall[i].y, poly[2].x, poly[2].y, 30) == TRUE) {
							fallCheck[i].direction = 2;
							fall[i].x = poly[2].x;
							fall[i].y = poly[2].y;
						}
					}
					else if (fallCheck[i].direction == 2)
					{
						if (poly[2].x < poly[3].x && poly[2].y < poly[3].y) {
							fall[i].x += abs(poly[2].x - poly[3].x) / 20;
							fall[i].y += abs(poly[2].y - poly[3].y) / 20;
						}
						else if (poly[2].x > poly[3].x && poly[2].y > poly[3].y) {
							fall[i].x -= abs(poly[2].x - poly[3].x) / 20;
							fall[i].y -= abs(poly[2].y - poly[3].y) / 20;
						}
						else if (poly[2].x > poly[3].x && poly[2].y < poly[3].y) {
							fall[i].x -= abs(poly[2].x - poly[3].x) / 20;
							fall[i].y += abs(poly[2].y - poly[3].y) / 20;
						}
						else if (poly[2].x < poly[3].x && poly[2].y > poly[3].y) {
							fall[i].x += abs(poly[2].x - poly[3].x) / 20;
							fall[i].y -= abs(poly[2].y - poly[3].y) / 20;
						}
						if (InCircle(fall[i].x, fall[i].y, poly[3].x, poly[3].y, 30) == TRUE) {
							fallCheck[i].direction = 3;
							fall[i].x = poly[3].x;
							fall[i].y = poly[3].y;
						}
					}
					else if (fallCheck[i].direction == 3)
					{
						if (poly[3].x < poly[4].x && poly[3].y < poly[4].y) {
							fall[i].x += abs(poly[3].x - poly[4].x) / 20;
							fall[i].y += abs(poly[3].y - poly[4].y) / 20;
						}
						else if (poly[3].x > poly[4].x && poly[3].y > poly[4].y) {
							fall[i].x -= abs(poly[3].x - poly[4].x) / 20;
							fall[i].y -= abs(poly[3].y - poly[4].y) / 20;
						}
						else if (poly[3].x > poly[4].x && poly[3].y < poly[4].y) {
							fall[i].x -= abs(poly[3].x - poly[4].x) / 20;
							fall[i].y += abs(poly[3].y - poly[4].y) / 20;
						}
						else if (poly[3].x < poly[4].x && poly[3].y > poly[4].y) {
							fall[i].x += abs(poly[3].x - poly[4].x) / 20;
							fall[i].y -= abs(poly[3].y - poly[4].y) / 20;
						}
						if (InCircle(fall[i].x, fall[i].y, poly[4].x, poly[4].y, 30) == TRUE) {
							fallCheck[i].direction = 4;
							fall[i].x = poly[4].x;
							fall[i].y = poly[4].y;
						}
					}
					else if (fallCheck[i].direction == 4)
					{
						if (poly[4].x < poly[0].x && poly[4].y < poly[0].y) {
							fall[i].x += abs(poly[4].x - poly[0].x) / 20;
							fall[i].y += abs(poly[4].y - poly[0].y) / 20;
						}
						else if (poly[4].x > poly[0].x && poly[4].y > poly[0].y) {
							fall[i].x -= abs(poly[4].x - poly[0].x) / 20;
							fall[i].y -= abs(poly[4].y - poly[0].y) / 20;
						}
						else if (poly[4].x > poly[0].x && poly[4].y < poly[0].y) {
							fall[i].x -= abs(poly[4].x - poly[0].x) / 20;
							fall[i].y += abs(poly[4].y - poly[0].y) / 20;
						}
						else if (poly[4].x < poly[0].x && poly[4].y > poly[0].y) {
							fall[i].x += abs(poly[4].x - poly[0].x) / 20;
							fall[i].y -= abs(poly[4].y - poly[0].y) / 20;
						}
						if (InCircle(fall[i].x, fall[i].y, poly[0].x, poly[0].y, 30) == TRUE) {
							fallCheck[i].direction = 0;
							fall[i].x = poly[0].x;
							fall[i].y = poly[0].y;
						}
					}

				}
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_KEYDOWN:

		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, 1280, 720);
		hbmMemOld = (HBITMAP)SelectObject(memDC, hBitmap);
		PatBlt(memDC, 0, 0, 1280, 720, BLACKNESS);
		SelectObject(memDC, (HBITMAP)hBitmap);
		myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		hBrush = CreateSolidBrush(RGB(100, 200, 100));
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		oldPen = (HPEN)SelectObject(memDC, myPen);
		Polygon(memDC, poly, 5);
		DeleteObject(hBrush);
		
		for (int i = 0; i < 10; ++i) {
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
			Ellipse(memDC, fall[i].x - 30, fall[i].y - 30, fall[i].x + 30, fall[i].y + 30);
			DeleteObject(hBrush);
		}

		BitBlt(hDC, 0, 0, 1280, 720, memDC, 0, 0, SRCCOPY);

		DeleteDC(memDC);
		DeleteObject(hbmMemOld);
		DeleteDC(hDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, IParam));
}


