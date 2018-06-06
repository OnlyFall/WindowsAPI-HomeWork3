#include <Windows.h>
#include <random>
#include<time.h>
#include<stdlib.h>
#include<tchar.h>
#include<math.h>

#define WideSize 900   
#define HighSize 900
#define Line 10
#define MAX 100
#define PIE 3.14


HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	//µÎ »öÀÇ ÇÕÀÌ 0À¸·Î µÇ°Ú
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "window  1-2", WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_HSCROLL | WS_VSCROLL | WS_SYSMENU, 0, 0, WideSize, HighSize, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
//BOOL InCircle(int x, int y, int mx, int my) { 
//	if (LengthPts(x, y, mx, my) < BSIZE)   
//		return TRUE;
//	else 
//		return FALSE; 
//}


float LengthPts(int x1, int y1, int x2, int y2) {
	return (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}

struct RandCircle {
	int x;
	int y;
}RC;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static SIZE size;
	HDC hdc;
	//drawtext¼±¾ðÀ» À§ÇÑ rect
	HPEN hpen, hpen2, oldpen, oldpen2;
	HBRUSH hBrush, oldBrush, hBrush2;
	PAINTSTRUCT ps;
	static RandCircle feed[100];
	static RandCircle Bomb[100];
	static int x[100] = { 0 };
	static int y[100] = { 0 };
	static int check, count = 0;
	static int direction = 0;
	static int o = 0;
	static int bc = 0;
	static int checkRandDirect;
	static int speed = 200;
	static int jumpCheck = 0;
	static int checkHead = 0;
	static int ¶ÈµüÀÌ = 0;
	static int headSize = 0;
	static int clickX = 0;
	static int clickY = 0;
	static int click±ôºýÀÌ = 0;
	static int testX;
	static int testY;
	static int minustestX;
	static int minustestY;
	static int mouseX;
	static int mouseY;
	static int movec = 0;
	static int Ã¼Å©ÇÔ = 0;

	static BOOL Selection[100];
	//»ç°¢Çü ¿ø ÁÂÇ¥ÀÌµ¿À» À§ÇÑ °ª
	switch (iMessage)
	{
	case WM_CREATE:
		x[0] = 12;
		y[0] = 12;
		SetTimer(hWnd, 5, 2000, NULL);
		//	SetTimer(hWnd, 6, 4000, NULL);
		SetTimer(hWnd, 10, 4000, NULL);
		//	SetTimer(hWnd, 11, 4000, NULL);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hpen);



		Rectangle(hdc, 0, 0, 800, 800);
		for (int i = 0; i < 40; i++) {
			MoveToEx(hdc, 20 * (i + 1), 0, NULL);
			LineTo(hdc, 20 * (i + 1), 800);
		}
		for (int i = 0; i < 40; i++) {
			MoveToEx(hdc, 0, 20 * (i + 1), NULL);
			LineTo(hdc, 800, 20 * (i + 1));
		}

		for (int i = 0; i < o; ++i)
		{
			if (x[0] == feed[i].x && y[0] == feed[i].y) {
				count++;
				for (int j = i; j < o; ++j)
				{
					feed[j].x = feed[j + 1].x;
					feed[j].y = feed[j + 1].y;
					o--;
				}
			}
		}

		for (int j = 0; j < bc; ++j)
		{
			if (x[0] == Bomb[j].x && y[0] == Bomb[j].y) {
				count--;
				for (int i = j; i < bc; ++i)
				{
					Bomb[i].x = Bomb[i + 1].x;
					Bomb[i].y = Bomb[i + 1].y;
					bc--;
				}
			}
		}


		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hBrush);
		Ellipse(hdc, x[0] * 20, y[0] * 20, x[0] * 20 + headSize + 20, y[0] * 20 + headSize + 20);
		for (int i = 0; i <= count; ++i)
			Ellipse(hdc, x[i] * 20, y[i] * 20, x[i] * 20 + 20, y[i] * 20 + 20);

		hBrush2 = CreateSolidBrush(RGB(255, 0, 0));
		oldpen = (HPEN)SelectObject(hdc, hBrush2);
		/*if (Selection[movec] == TRUE) {
		Ellipse(hdc, feed[movec].x , feed[movec].y, feed[movec].x+ 20, feed[movec].y + 20);
		}*/
		for (int j = 0; j < o; ++j) {
			Ellipse(hdc, feed[j].x * 20, feed[j].y * 20, feed[j].x * 20 + 20, feed[j].y * 20 + 20);
		}
		for (int k = 0; k < bc; ++k)
			Rectangle(hdc, Bomb[k].x * 20, Bomb[k].y * 20, Bomb[k].x * 20 + 20, Bomb[k].y * 20 + 20);
		EndPaint(hWnd, &ps);
		break;


	case WM_KEYDOWN:
		if (wParam == VK_RIGHT) {
			SetTimer(hWnd, 1, speed, NULL);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			direction = 1;
		}
		else if (wParam == VK_LEFT) {
			SetTimer(hWnd, 2, speed, NULL);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			direction = 2;
		}
		else if (wParam == VK_UP) {
			SetTimer(hWnd, 3, speed, NULL);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 4);
			direction = 3;
		}
		else if (wParam == VK_DOWN) {
			SetTimer(hWnd, 4, speed, NULL);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 1);
			direction = 4;
		}
		else if (wParam == VK_SPACE) {
			if (direction == 1 || direction == 2)
				SetTimer(hWnd, 7, speed, NULL);
			else
				SetTimer(hWnd, 8, speed, NULL);
		}
		else if (wParam == VK_ESCAPE)
		{
			if (checkHead % 2 == 1)
				SetTimer(hWnd, 9, speed - 100, NULL);
			else if (checkHead % 2 == 0)
				KillTimer(hWnd, 9);
			checkHead++;
		}
		break;

	case WM_CHAR:
		if (wParam == '+') {
			speed -= 50;
			if (direction == 1) {
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speed, NULL);
			}
			else if (direction == 2) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, speed, NULL);
			}
			else if (direction == 3) {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 3, speed, NULL);
			}
			else if (direction == 4) {
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 4, speed, NULL);
			}

		}
		else if (wParam == '-') {
			speed += 50;
			if (direction == 1) {
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speed, NULL);
			}
			else if (direction == 2) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, speed, NULL);
			}
			else if (direction == 3) {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 3, speed, NULL);
			}
			else if (direction == 4) {
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 4, speed, NULL);
			}

		}
		break;

	case WM_RBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);
		if (InCircle(mouseX, mouseY, x[0] * 20 + 10, y[0] * 20 + 10, 10) == TRUE)
		{
			if (click±ôºýÀÌ % 2 == 0)
				SetTimer(hWnd, 9, speed - 100, NULL);
			else if (click±ôºýÀÌ % 2 == 1)
				KillTimer(hWnd, 9);
			click±ôºýÀÌ++;
		}
		break;

	case WM_RBUTTONDBLCLK:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);


		break;


	case  WM_LBUTTONDOWN:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		if (mouseX - (x[0] * 20) >= 0 && mouseY - (y[0] * 20) >= 0)
		{
			if (mouseX - (x[0] * 20) > mouseY - (y[0] * 20))
			{
				SetTimer(hWnd, 1, speed, NULL);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
			}
			else {
				SetTimer(hWnd, 4, speed, NULL);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
			}
		}

		else if (mouseX - (x[0] * 20) >= 0 && mouseY - (y[0] * 20) < 0) {
			if (mouseX - (x[0] * 20) > (mouseY - (y[0] * 20)) * (-1)) {
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, speed, NULL);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
			}
			else {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 3, speed, NULL);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 4);
			}
		}
		else if (mouseX - (x[0] * 20) < 0 && mouseY - (y[0] * 20) < 0) {
			if ((mouseX - (x[0] * 20)) * (-1) > (mouseY - (y[0] * 20)) * (-1)) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, speed, NULL);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
			}
			else {
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 3, speed, NULL);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 4);
			}
		}
		else if (mouseX - (x[0] * 20) < 0 && mouseY - (y[0] * 20) >= 0) {
			if ((mouseX - (x[0] * 20)) * (-1) > mouseY - (y[0] * 20)) {
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 2, speed, NULL);
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
			}
			else {
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 4, speed, NULL);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
			}
		}

		if (InCircle(mouseX, mouseY, x[0] * 20 + 10, y[0] * 20 + 10, 10) == TRUE) {
			if (Ã¼Å©ÇÔ % 2 == 0) {
				KillTimer(hWnd, 1);
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				KillTimer(hWnd, 4);
				SetTimer(hWnd, 6, 1000, NULL);
				SetTimer(hWnd, 11, 1000, NULL);
				Ã¼Å©ÇÔ++;
			}
			else if (Ã¼Å©ÇÔ % 2 == 1) {
				KillTimer(hWnd, 6);
				KillTimer(hWnd, 11);
				SetTimer(hWnd, 1, speed, NULL);
				Ã¼Å©ÇÔ++;
			}
		}

		for (int i = 0; i < o; ++i) {
			if (InCircle(feed[i].x * 20 + 10, feed[i].y * 20 + 10, mouseX, mouseY, 10)) {
				Selection[i] = TRUE;
				movec = i;

				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
		}



		break;
	case WM_LBUTTONUP:
		//InvalidateRect(hWnd, NULL, TRUE);
		//Selection = FALSE;
		InvalidateRect(hWnd, NULL, TRUE);
		Selection[movec] = FALSE;
		break;

	case WM_MOUSEMOVE:
		mouseX = LOWORD(lParam);
		mouseY = HIWORD(lParam);

		if (Selection[movec]) {
			feed[movec].x = mouseX / 20;
			feed[movec].y = mouseY / 20;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			for (int i = count; i > 0; --i) {
				x[i] = x[i - 1];
				y[i] = y[i - 1];

			}
			x[0] += 1;
			if (x[0] * 20 >= 800)
				x[0] -= 1;
			break;

		case 2:
			for (int i = count; i > 0; --i) {
				x[i] = x[i - 1];
				y[i] = y[i - 1];
			}
			x[0] -= 1;
			if (x[0] * 20 < 0)
				x[0] += 1;
			break;

		case 3:
			for (int i = count; i > 0; --i) {
				x[i] = x[i - 1];
				y[i] = y[i - 1];
			}
			y[0] -= 1;
			if (y[0] * 20 < 0)
				y[0] += 1;
			break;

		case 4:
			for (int i = count; i > 0; --i) {
				x[i] = x[i - 1];
				y[i] = y[i - 1];
			}
			y[0] += 1;
			if (y[0] * 20 >= 800)
				y[0] -= 1;
			break;

		case 5:
			feed[o].x = rand() % 40;
			feed[o].y = rand() % 40;
			o++;
			break;

		case 6:
			for (int k = 0; k <= o; ++k)
			{
				checkRandDirect = rand() % 4;
				if (checkRandDirect == 0)
					feed[k].x += 1;
				else if (checkRandDirect == 1)
					feed[k].x -= 1;
				else if (checkRandDirect == 2)
					feed[k].y -= 1;
				else if (checkRandDirect == 3)
					feed[k].y += 1;
			}
			break;

		case 7:
			jumpCheck++;
			if (jumpCheck <= 2) {
				for (int i = 0; i <= count; i++) {
					y[i] -= 1;
				}
			}
			else if (jumpCheck > 2 && jumpCheck <= 4)
			{
				for (int i = 0; i <= count; i++) {
					y[i] += 1;
				}
			}
			if (jumpCheck == 4)
			{
				jumpCheck = 0;
				KillTimer(hWnd, 7);
			}
			break;

		case 8:
			jumpCheck++;
			if (jumpCheck <= 2) {
				for (int i = 0; i <= count; i++) {
					x[i] += 1;
				}
			}
			else if (jumpCheck > 2 && jumpCheck <= 4)
			{
				for (int i = 0; i <= count; i++) {
					x[i] -= 1;
				}
			}
			if (jumpCheck == 4)
			{
				jumpCheck = 0;
				KillTimer(hWnd, 8);
			}
			break;

		case 9:
			if (¶ÈµüÀÌ % 2 == 1)
				headSize -= 10;
			else if (¶ÈµüÀÌ % 2 == 0)
				headSize += 10;
			¶ÈµüÀÌ++;
			break;

		case 10:
			Bomb[bc].x = rand() % 40;
			Bomb[bc].y = rand() % 40;
			bc++;
			break;

		case 11:
			for (int k = 0; k <= bc; ++k)
			{
				checkRandDirect = rand() % 4;
				if (checkRandDirect == 0)
					Bomb[k].x += 1;
				else if (checkRandDirect == 1)
					Bomb[k].x -= 1;
				else if (checkRandDirect == 2)
					Bomb[k].y -= 1;
				else if (checkRandDirect == 3)
					Bomb[k].y += 1;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;


	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}