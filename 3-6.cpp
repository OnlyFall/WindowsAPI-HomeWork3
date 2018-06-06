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
}; // 음식은 x,y좌표, 충돌여부, 반지름을 갖는다.


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
LPCTSTR lpszClass = TEXT("First");

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

	hWnd = CreateWindow(lpszClass, "우빈이왔쪄염 뿌우", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hinstance, NULL);
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
	switch (iMessage) //메시지의 번호
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
		if (Drag) {      // 흰 바탕   
			SetROP2(hdc, R2_XORPEN);   // 펜의 XOR 연산  
			SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN)); // 흰 펜        // 흰 바탕 XOR 흰 펜 = 검은색 펜  
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);   // 지우기 : 흰 바탕 XOR 검은 펜 = 흰 
			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY; // 현 지점을 이전 지점으로 설정   	
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