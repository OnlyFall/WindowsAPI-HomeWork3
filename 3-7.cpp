//#include <Windows.h>
//#include <iostream>
//#include <random>
//#include <time.h>
//#define Height 1280
//#define Weight 720
//
//using namespace std;
//HPEN MyPen, OldPen;
//#define MAX 9
//HINSTANCE g_hInst;
//LPCTSTR lpszClass = "Windows Programing KWB";
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
//{
//	HWND hWnd;
//	MSG Message;
//	WNDCLASSEX WndClass;
//	g_hInst = hInstance;
//	WndClass.cbSize = sizeof(WndClass);
//	WndClass.style = CS_HREDRAW | CS_VREDRAW;
//	WndClass.lpfnWndProc = (WNDPROC)WndProc;
//	WndClass.cbClsExtra = 0;
//	WndClass.cbWndExtra = 0;
//	WndClass.hInstance = hInstance;
//	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	WndClass.lpszMenuName = NULL;
//	WndClass.lpszClassName = lpszClass;
//	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//	RegisterClassEx(&WndClass);
//	hWnd = CreateWindow(lpszClass, "window progra1-2", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 100, 50, Height, Weight, NULL, (HMENU)NULL, hInstance, NULL);
//	int x = 800;
//	int y = 600;
//
//	ShowWindow(hWnd, nCmdShow);
//	UpdateWindow(hWnd);
//	while (GetMessage(&Message, 0, 0, 0)) {
//		TranslateMessage(&Message);
//		DispatchMessage(&Message);
//	}
//	return Message.wParam;
//}
//static char str;
//int p = 0;
//int position[100];
//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam) {
//	HDC hDC;
//	PAINTSTRUCT ps;
//	static TCHAR stra[100][100] = { NULL };
//	static TCHAR CARET[100][100];
//
//	switch (iMessage) {
//	case WM_CREATE:
//		
//
//		return 0;
//	case WM_CHAR:
//		break;
//	case WM_KEYDOWN:
//		
//		break;
//
//	case WM_PAINT:
//		hDC = BeginPaint(hWnd, &ps);
//	
//
//		EndPaint(hWnd, &ps);
//		return 0;
//
//	case WM_DESTROY:
//
//		PostQuitMessage(0);
//		return 0;
//	}
//	return(DefWindowProc(hWnd, iMessage, wParam, IParam));
//}
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

double checkdistance(int x, int y, int mx, int my);
BOOL inCircle(int x, int y, int mx, int my, int size);

struct Circle
{
	int x, y;
	int r; // ������
	BOOL check;
	BOOL istrue;
	int angle;
	int hitcount = 0;
	double rad;
};


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h�� �ǹ�? = �ڵ�
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //���α׷� ������ ���̴� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������ 
	WndClass.hInstance = hinstance; //���� ����ǰ� �ִ� ��ü�� �ڵ�
	WndClass.lpfnWndProc = WndProc; //���ν����Լ��� �̸� 
	WndClass.lpszClassName = lpszClass; //������ Ŭ���� ������ �̸� 
	WndClass.lpszMenuName = NULL; // �޴��� �̸�
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //������ ��� ��Ÿ��
											  // ������� WNDCLASS����ü�� �����鿡 ���� ����

	RegisterClass(&WndClass); // ������ Ŭ������ �ü���� ���

	hWnd = CreateWindow(lpszClass, "����̿��ǿ� �ѿ�", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(Ŭ���� �̸�,Ÿ��Ʋ �̸�,������ ��Ÿ��, ������ ��ġ��ǥx , y,������ ����ũ��, ������ ����ũ��, �θ� ������ �ڵ�, �޴� �ڵ�, �������α׷� �ν��Ͻ�, ���� ������ ����(NULL))
	ShowWindow(hWnd, nCmdShow); //�������� ȭ�� ���
	UpdateWindow(hWnd); //OS�� WM_PAINT�޽��� ����
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//������ ���ν������� PostQuitMessage()�� ȣ���Ҷ� �����.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//�޽��������� ����.
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(������ �ڵ�, ó���ؾ� �� �޽����� ��,�Է���ġ�� ��1,2)
{
	PAINTSTRUCT ps;
	static HDC hdc;
	HPEN hpen, oldpen;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hbmOld, hbmMem, hbmMemOld;
	static HDC memDC;
	static HBITMAP hBitmap;
	static Circle hero;
	static Circle map[20];
	static char buf[50];
	static int count = 0;
	static BOOL select;
	srand(unsigned(time));
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CHAR:
		if (wParam == 'e' || wParam == 'E')
			goto r1;
		break;

	case WM_LBUTTONDOWN:
		select = TRUE;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_MOUSEMOVE:
		if (select == TRUE)
		{
			hero.x = LOWORD(lParam);
			hero.y = HIWORD(lParam);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONUP:
		hero.x = LOWORD(lParam);
		hero.y = HIWORD(lParam);
		select = FALSE;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_CREATE:
	r1:
		srand((unsigned)time(NULL));
		for (int i = 0; i < 20; ++i) {
			map[i].hitcount = 0;
			count = 0;
		}
		SetTimer(hWnd, 1, 5, NULL);
		hero.x = 0;
		hero.y = 0;
		hero.r = 50;
		for (int i = 0; i < 20; i++)
		{
			map[i].x = rand() % 1100;
			map[i].y = rand() % 580;
			map[i].r = rand() % 100;
			for (int i = 0; i < 20; ++i)
				map[i].check = rand() % 3;
			map[i].istrue = TRUE;
		}
		break;
	case WM_TIMER:
		// ���� ����
		switch (wParam) {
		case 1:
			for (int i = 0; i < 20; ++i) {
				map[i].angle++;
				map[i].rad = 3.14 * map[i].angle / 180.0;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT: //�޽����� ó��
		hdc = BeginPaint(hWnd, &ps);
		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(memDC, 1280, 720);
		hbmMemOld = (HBITMAP)SelectObject(memDC, hBitmap);
		PatBlt(memDC, 0, 0, 1280, 720, WHITENESS);
		SelectObject(memDC, (HBITMAP)hBitmap);
		
		hBrush = CreateSolidBrush(RGB(0, 0, 250));
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
		
		Ellipse(memDC, hero.x - hero.r, hero.y - hero.r, hero.x + hero.r, hero.y + hero.r);
		DeleteObject(oldBrush);

		for (int i = 0; i < 20; i++)
		{
			if (map[i].istrue == TRUE && inCircle(hero.x, hero.y, map[i].x, map[i].y, hero.r) == TRUE)
			{
				map[i].hitcount = 1;
				if (map[i].check == 1)
					map[i].check = 0;
				else if (map[i].check == 0)
					map[i].check = 1;
				else if (map[i].check == 2)
					map[i].check = 0;
				map[i].istrue = FALSE;
				count++;
			}
		}
		for (int i = 0; i < 20; i++)
		{
			if (map[i].check == 2 && map[i].hitcount == 0) {
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				Ellipse(memDC, map[i].x - map[i].r, map[i].y - map[i].r, map[i].x + map[i].r, map[i].y + map[i].r);
				MoveToEx(memDC, map[i].x - map[i].r*cos(0), map[i].y - map[i].r*sin(0), NULL);
				LineTo(memDC, map[i].x + map[i].r*cos(0), map[i].y + map[i].r*sin(0));
				MoveToEx(memDC, map[i].x + map[i].r*cos(360), map[i].y - map[i].r*sin(360), NULL);
				LineTo(memDC, map[i].x - map[i].r*cos(360), map[i].y + map[i].r*sin(360));
				DeleteObject(oldBrush);
			}
			else if (map[i].check == 0 && map[i].hitcount == 0)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				Ellipse(memDC, map[i].x - map[i].r, map[i].y - map[i].r, map[i].x + map[i].r, map[i].y + map[i].r);
				DeleteObject(oldBrush);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				MoveToEx(memDC, map[i].x - map[i].r*cos(map[i].rad), map[i].y - map[i].r*sin(map[i].rad), NULL);
				LineTo(memDC, map[i].x + map[i].r*cos(map[i].rad), map[i].y + map[i].r*sin(map[i].rad));
				MoveToEx(memDC, map[i].x + map[i].r*cos(360 - map[i].rad), map[i].y - map[i].r*sin(360 - map[i].rad), NULL);
				LineTo(memDC, map[i].x - map[i].r*cos(360 - map[i].rad), map[i].y + map[i].r*sin(360 - map[i].rad));
				DeleteObject(oldBrush);
			}
			else if (map[i].check == 0 && map[i].hitcount == 1)
			{
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				Ellipse(memDC, map[i].x - map[i].r, map[i].y - map[i].r, map[i].x + map[i].r, map[i].y + map[i].r);
				DeleteObject(oldBrush);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				MoveToEx(memDC, map[i].x - map[i].r*cos(map[i].rad), map[i].y - map[i].r*sin(map[i].rad), NULL);
				LineTo(memDC, map[i].x + map[i].r*cos(map[i].rad), map[i].y + map[i].r*sin(map[i].rad));
				MoveToEx(memDC, map[i].x + map[i].r*cos(360 - map[i].rad), map[i].y - map[i].r*sin(360 - map[i].rad), NULL);
				LineTo(memDC, map[i].x - map[i].r*cos(360 - map[i].rad), map[i].y + map[i].r*sin(360 - map[i].rad));
				DeleteObject(oldBrush);
			}
			else if (map[i].check == 1 && map[i].hitcount == 0)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				Ellipse(memDC, map[i].x - map[i].r, map[i].y - map[i].r, map[i].x + map[i].r, map[i].y + map[i].r);
				DeleteObject(oldBrush);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				MoveToEx(memDC, map[i].x + map[i].r*cos(map[i].rad), map[i].y - map[i].r*sin(map[i].rad), NULL);
				LineTo(memDC, map[i].x - map[i].r*cos(map[i].rad), map[i].y + map[i].r*sin(map[i].rad));
				MoveToEx(memDC, map[i].x - map[i].r*cos(360 - map[i].rad), map[i].y - map[i].r*sin(360 - map[i].rad), NULL);
				LineTo(memDC, map[i].x + map[i].r*cos(360 - map[i].rad), map[i].y + map[i].r*sin(360 - map[i].rad));
				DeleteObject(oldBrush);
			}
			else if (map[i].check == 1 && map[i].hitcount == 1)
			{
				hBrush = CreateSolidBrush(RGB(255, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				Ellipse(memDC, map[i].x - map[i].r, map[i].y - map[i].r, map[i].x + map[i].r, map[i].y + map[i].r);
				DeleteObject(oldBrush);
				hBrush = CreateSolidBrush(RGB(0, 0, 0));
				oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
				MoveToEx(memDC, map[i].x + map[i].r*cos(map[i].rad), map[i].y - map[i].r*sin(map[i].rad), NULL);
				LineTo(memDC, map[i].x - map[i].r*cos(map[i].rad), map[i].y + map[i].r*sin(map[i].rad));
				MoveToEx(memDC, map[i].x - map[i].r*cos(360 - map[i].rad), map[i].y - map[i].r*sin(360 - map[i].rad), NULL);
				LineTo(memDC, map[i].x + map[i].r*cos(360 - map[i].rad), map[i].y + map[i].r*sin(360 - map[i].rad));
				DeleteObject(oldBrush);
			}
		}
		if (hero.x > 1200 && hero.y > 600)
		{
			wsprintf(buf, "�ε��� ����:%d", count);
			TextOut(memDC, 640, 300, buf, strlen(buf));
			KillTimer(hWnd, 1);
		}

		BitBlt(hdc, 0, 0, 1280, 720, memDC, 0, 0, SRCCOPY);

		DeleteDC(memDC);
		DeleteObject(hbmMemOld);
		DeleteDC(hdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	//case���� ���ǵ��� ���� �޽����� Ŀ���� ó���ϵ��� �޽��� ����
}

double checkdistance(int x, int y, int mx, int my)
{
	return sqrt((mx - x)*(mx - x) + (my - y)*(my - y));
}

BOOL inCircle(int x, int y, int mx, int my, int size)
{
	if (checkdistance(x, y, mx, my) < size)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}