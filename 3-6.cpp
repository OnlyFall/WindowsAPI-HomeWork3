#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

BOOL InCircle(int, int, int, int, int);
double LengthCheck(int, int, int, int);
float Check(int, int, int, int, int);
struct Circle {
	int x, y;
	int r;
	int R, G, B;
	BOOL isTrue;
}; // ������ x,y��ǥ, �浹����, �������� ���´�.


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

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

	hWnd = CreateWindow(lpszClass, "����̿��ǿ� �ѿ�", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
	HDC hdc;
	static RECT rect;
	HPEN myPen;
	HPEN oldPen;
	HBRUSH hbrush;
	HBRUSH oldbrush;
	static Circle circle[10];
	static int CircleCount = 0;
	static int startX, startY, oldX, oldY;
	static BOOL Drag, Drag2;
	static int endX, endY;
	int saveX, saveY;
	static int circleX, circleY;
	static int check;
	static int selectCircle;
	static bool xflag = true;
	switch (iMessage) //�޽����� ��ȣ
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 2000, NULL);

		Drag = FALSE;

		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (CircleCount < 9) {
				circle[CircleCount].x = rand() % 800;
				circle[CircleCount].y = rand() % 800;
				circle[CircleCount].r = 40;
				circle[CircleCount].R = rand() % 255;
				circle[CircleCount].G = rand() % 255;
				circle[CircleCount].B = rand() % 255;
				CircleCount++;
			}
			break;

		case 2:
			for (int i = 0; i <= CircleCount; ++i) {
				circle[i].r -= 4;
				if (circle[i].r == 0) {
					circle[i].x = rand() % 800;
					circle[i].y = rand() % 800;
					circle[i].r = 40;
					circle[i].R = rand() % 255;
					circle[i].G = rand() % 255;
					circle[i].B = rand() % 255;
				}
			}
			break;

		case 3:
			if (startX - endX < 0 && startY - endY < 0) {
				if (circle[check].x != endX)
					circle[check].x += abs(startX - endX) / 10;
				if (circle[check].y != endY)
					circle[check].y += abs(startY - endY) / 10;

				if (InCircle(circle[check].x, circle[check].y, endX, endY, circle[check].r) == TRUE)
					KillTimer(hWnd, 3);
			}

			else if (startX - endX > 0 && startY - endY > 0) {
				if (circle[check].x != endX)
					circle[check].x -= abs(startX - endX) / 10;;
				if (circle[check].y != endY)
					circle[check].y -= abs(startY - endY) / 10;;

				if (InCircle(circle[check].x, circle[check].y, endX, endY, circle[check].r) == TRUE)
					KillTimer(hWnd, 3);
			}

			else if (startX - endX > 0 && startY - endY < 0) {
				if (circle[check].x != endX)
					circle[check].x -= abs(startX - endX) / 10;;
				if (circle[check].y != endY)
					circle[check].y += abs(startY - endY) / 10;;

				if (InCircle(circle[check].x, circle[check].y, endX, endY, circle[check].r) == TRUE)
					KillTimer(hWnd, 3);
			}
			else if (startX - endX < 0 && startY - endY > 0) {
				if (circle[check].x != endX)
					circle[check].x += abs(startX - endX) / 10;;
				if (circle[check].y != endY)
					circle[check].y -= abs(startY - endY) / 10;;
				if (InCircle(circle[check].x, circle[check].y, endX, endY, circle[check].r) == TRUE)
					KillTimer(hWnd, 3);
			}
			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		startX = LOWORD(lParam);
		startY = HIWORD(lParam);
		Drag = TRUE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		circleX = LOWORD(lParam);
		circleY = HIWORD(lParam);
		if (Drag) {      // �� ����   
			SetROP2(hdc, R2_XORPEN);   // ���� XOR ����  
			SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN)); // �� ��        // �� ���� XOR �� �� = ������ ��  
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);   // ����� : �� ���� XOR ���� �� = �� 
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY; // �� ������ ���� �������� ����   	
		}

		if (Drag2) {
			circle[selectCircle].x = circleX;
			circle[selectCircle].y = circleY;
			if (selectCircle == check) {
				KillTimer(hWnd, 3);
			}
		}
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_LBUTTONUP:
		endX = LOWORD(lParam);
		endY = HIWORD(lParam);
		Drag = FALSE;
		for (int i = 0; i <= CircleCount; ++i) {
			if (InCircle(circle[i].x, circle[i].y, startX, startY, circle[i].r) == TRUE) {
				check = i;
				SetTimer(hWnd, 3, 100, NULL);
			}
		}
		break;

	case WM_RBUTTONUP:
		Drag2 = FALSE;
		break;

	case WM_RBUTTONDOWN:
		circleX = LOWORD(lParam);
		circleY = HIWORD(lParam);

		for (int i = 0; i <= CircleCount; ++i) {
			if (InCircle(circle[i].x, circle[i].y, circleX, circleY, circle[i].r) == TRUE) {
				selectCircle = i;
				Drag2 = TRUE;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (circle[check].x != endX && circle[check].y != endY) {
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);
		}

		for (int i = 0; i < CircleCount; ++i) {
			hbrush = CreateSolidBrush(RGB(circle[i].R, circle[i].G, circle[i].B));
			oldbrush = (HBRUSH)SelectObject(hdc, hbrush);
			Ellipse(hdc, circle[i].x - circle[i].r, circle[i].y - circle[i].r, circle[i].x + circle[i].r, circle[i].y + circle[i].r);
			DeleteObject(hbrush);
		}

		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
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

double LengthCheck(int mx, int my, int x, int y)
{
	return sqrt((mx - x)*(mx - x) + (my - y)*(my - y));
}

float Check(int x, int y, int lx, int ly, int mx)
{
	return (float)(ly - y) / (float)(lx - x)*(float)(mx - lx) + (float)ly;
}