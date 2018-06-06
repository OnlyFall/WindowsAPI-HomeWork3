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
	int r; // 반지름
	BOOL check;
	BOOL istrue;
	int angle;
	int hitcount = 0;
	double rad;
};


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) //h의 의미? = 핸들
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hinstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //프로그램 내에서 보이는 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘 
	WndClass.hInstance = hinstance; //현재 실행되고 있는 객체의 핸들
	WndClass.lpfnWndProc = WndProc; //프로시저함수의 이름 
	WndClass.lpszClassName = lpszClass; //윈도우 클래스 내용의 이름 
	WndClass.lpszMenuName = NULL; // 메뉴바 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW; //윈도우 출력 스타일
											  // 여기까지 WNDCLASS구조체의 변수들에 값을 대입

	RegisterClass(&WndClass); // 윈도우 클래스를 운영체제에 등록

	hWnd = CreateWindow(lpszClass, "우빈이왔쪄염 뿌우", WS_OVERLAPPEDWINDOW, 0, 0, 1280, 720, NULL, (HMENU)NULL, hinstance, NULL);
	//CreateWindow(클래스 이름,타이틀 이름,윈도우 스타일, 윈도우 위치좌표x , y,윈도우 가로크기, 윈도우 세로크기, 부모 윈도우 핸들, 메뉴 핸들, 응용프로그램 인스턴스, 생성 윈도우 정보(NULL))
	ShowWindow(hWnd, nCmdShow); //윈도우의 화면 출력
	UpdateWindow(hWnd); //OS에 WM_PAINT메시지 전송
	while (GetMessage(&Message, NULL, 0, 0))
	{
		//윈도우 프로시저에서 PostQuitMessage()를 호출할때 종료됨.
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}//메시지루프를 돌림.
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) //CALLBACK(윈도우 핸들, 처리해야 할 메시지의 값,입력장치의 값1,2)
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
	switch (iMessage) //메시지의 번호
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
		// 각도 증가
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
	case WM_PAINT: //메시지의 처리
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
			wsprintf(buf, "부딪힌 갯수:%d", count);
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
	//case에서 정의되지 않은 메시지는 커널이 처리하도록 메시지 전달
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