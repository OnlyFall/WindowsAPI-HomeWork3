#include <windows.h>
#include <math.h>
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Test";
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	// 윈도우 클래스 구조체 값 설정 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject
	(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	// 윈도우 클래스 등록 
	RegisterClass(&WndClass);
	// 윈도우 생성 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);
	// 윈도우 출력 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//이베느 루프 처리 
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
#define PI 3.141592
struct DPOINT//좌표 구조체
{
	double x;
	double y;
};
static DPOINT center[4];//원 중심
double radius;//원 반지름
double angle[4];//점찍을 각도
double rectX[4] = { 0 };
double rectY[4] = { 0 };
double rectTx[4] = { 0 };
double rectTy[4] = { 0 };


DPOINT getPosition(DPOINT cen, double rad, double ang)
{
	DPOINT pt;
	//각도가 90 180 270도는 게산이 않되서 예외 처리
	if (ang == 90)
	{
		pt.x = cen.x;
		pt.y = radius + cen.y;
	}
	else if (ang == 180)
	{
		pt.x = -radius + cen.x;
		pt.y = cen.y;
	}
	else if (ang == 270)
	{
		pt.x = cen.x;
		pt.y = -radius + cen.y;
	}
	else
	{
		pt.x = cen.x + radius * cos(ang * PI / 180);
		pt.y = cen.y + radius * sin(ang * PI / 180);
	}
	return pt;
}

struct TriangleP {
	POINT triangle[3];
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, hPen2, hPen3, hPen4, oldPen;
	static int set;
	static int value = 1;
	static int valueRectangle[4] = { NULL };
	static int valueCircle[4] = { NULL };
	static int valueTriangle[4] = { NULL };
	static TriangleP Triangle[4];
	static DPOINT pt[4];

	static int rectangleCheck[4] = { 0 };
	static int rectangleMinus[4] = { 0 };
	static int rectangleYminus[4] = { 0 };

	static int TriangleMinus[4] = { 0 };
	static int TriangleCheck[4] = { 0 };

	static int TriangleP[4];
	static int inputC[4] = { 0 };
	switch (message)
	{
	case WM_CREATE:
		//배경도형이 원인경우와 원/ 사각형/ 삼각형 을 그릴때 사용
		center[0].x = 150;
		center[0].y = 150;
		center[1].x = 450;
		center[1].y = 150;
		center[2].x = 150;
		center[2].y = 450;
		center[3].x = 450;
		center[3].y = 450;

		//배경 도형이 사각형인 경우 사용
		rectX[0] = 50;
		rectY[0] = 50;
		rectX[1] = 350;
		rectY[1] = 50;
		rectX[2] = 50;
		rectY[2] = 350;
		rectX[3] = 350;
		rectY[3] = 350;

		//배경 도형이 삼각형인 경우 사용
		rectTx[3] = 350;
		rectTy[3] = 550;
		rectTx[2] = 50;
		rectTy[2] = 550;
		rectTx[1] = 350;
		rectTy[1] = 250;
		rectTx[0] = 50;
		rectTy[0] = 250;

		radius = 100;
		angle[0] = 0;
		angle[1] = 0;
		angle[2] = 0;
		angle[3] = 0;//타이머 시작

		break;

	case WM_KEYDOWN:
		if (wParam == '1') {
			SetTimer(hWnd, 1, 33, NULL);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			set = 0;
		}
		else if (wParam == '2') {
			SetTimer(hWnd, 2, 33, NULL);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 3);
			KillTimer(hWnd, 4);
			set = 1;
		}
		else if (wParam == '3') {
			SetTimer(hWnd, 3, 33, NULL);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 4);
			set = 2;
		}
		else if (wParam == '4') {
			SetTimer(hWnd, 4, 33, NULL);
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			KillTimer(hWnd, 3);
			set = 3;
		}
		else if (wParam == 'c' || wParam == 'C')
			inputC[set]++;
		else if (wParam == 'm' || wParam == 'M')
			valueCircle[set] += 1;
		else if (wParam == 'r' || wParam == 'R')
			valueRectangle[set] += 1;
		else if (wParam == 't' || wParam == 'T')
			valueTriangle[set] += 1;
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (valueRectangle[0] % 2 == 0 && valueTriangle[0] % 2 == 0) {
				if (inputC[0] % 2 == 0)
				{
					angle[0] += 1;
					if (angle[0] >= 360)
						angle[0] = 0;
					pt[0] = getPosition(center[0], radius, angle[0]);
				}
				else if (inputC[0] % 2 == 1)
				{
					angle[0] -= 1;
					if (angle[0] <= 0)
						angle[0] = 360;
					pt[0] = getPosition(center[0], radius, angle[0]);
				}
			}

			else if (valueRectangle[0] % 2 == 1)
			{
				if (inputC[0] % 2 == 0) {
					if (rectangleCheck[0] % 2 == 0)
					{
						if (rectangleMinus[0] % 2 == 0) {
							rectX[0] += 5;
							if (rectX[0] >= 250) {
								rectangleMinus[0]++;
								rectangleCheck[0]++;
							}
						}
						else if (rectangleMinus[0] % 2 == 1) {
							rectX[0] -= 5;
							if (rectX[0] <= 50) {
								rectangleMinus[0]++;
								rectangleCheck[0]++;
							}
						}
					}
					else if (rectangleCheck[0] % 2 == 1) {
						if (rectangleYminus[0] % 2 == 0)
						{
							rectY[0] += 5;
							if (rectY[0] >= 250) {
								rectangleYminus[0]++;
								rectangleCheck[0]++;
							}
						}
						else if (rectangleYminus[0] % 2 == 1) {
							rectY[0] -= 5;
							if (rectY[0] <= 50) {
								rectangleYminus[0]++;
								rectangleCheck[0]++;
							}

						}
					}
				}
				else if (inputC[0] % 2 == 1)
				{
					if (rectangleCheck[0] % 2 == 0) {
						if (rectangleYminus[0] % 2 == 0) {
							rectY[0] += 5;
							if (rectY[0] >= 250) {
								rectangleYminus[0]++;
								rectangleCheck[0]++;
							}
						}
						else if (rectangleYminus[0] % 2 == 1) {
							rectY[0] -= 5;
							if (rectY[0] <= 50) {
								rectangleYminus[0]++;
								rectangleCheck[0]++;
							}
						}
					}
					else if (rectangleCheck[0] % 2 == 1) {
						if (rectangleMinus[0] % 2 == 0) {
							rectX[0] += 5;
							if (rectX[0] >= 250) {
								rectangleMinus[0]++;
								rectangleCheck[0]++;
							}
						}
						else if (rectangleMinus[0] % 2 == 1) {
							rectX[0] -= 5;
							if (rectX[0] <= 50) {
								rectangleMinus[0]++;
								rectangleCheck[0]++;
							}
						}

					}
				}
			}
			else if (valueTriangle[0] % 2 == 1) {
				if (inputC[0] % 2 == 0) {
					if (TriangleCheck[0] % 3 == 0)
					{
						rectTx[0] += 5;
						rectTy[0] -= 10;
						if (rectTy[0] <= 50)
							TriangleCheck[0]++;
					}
					else if (TriangleCheck[0] % 3 == 1)
					{
						rectTx[0] += 5;
						rectTy[0] += 10;
						if (rectTy[0] >= 250)
							TriangleCheck[0]++;
					}
					else if (TriangleCheck[0] % 3 == 2)
					{
						rectTx[0] -= 5;
						if (rectTx[0] <= 50)
							TriangleCheck[0]++;
					}
				}
				else if (inputC[0] % 2 == 1) {
					if (TriangleCheck[0] % 3 == 0)
					{
						rectTx[0] += 5;
						if (rectTx[0] >= 250)
							TriangleCheck[0]++;
					}
					else if (TriangleCheck[0] % 3 == 1)
					{
						rectTx[0] -= 5;
						rectTy[0] -= 10;
						if (rectTy[0] <= 50)
							TriangleCheck[0]++;
					}
					else if (TriangleCheck[0] % 3 == 2)
					{
						rectTx[0] -= 5;
						rectTy[0] += 10;
						if (rectTy[0] >= 250)
							TriangleCheck[0]++;
					}
				}
			}

			break;

		case 2:
			if (valueRectangle[1] % 2 == 0 && valueTriangle[1] % 2 == 0) {
				if (inputC[1] % 2 == 0) {
					angle[1] += 1;
					if (angle[1] >= 360)
						angle[1] = 0;
					pt[1] = getPosition(center[1], radius, angle[1]);
				}
				else if (inputC[1] % 2 == 1) {
					angle[1] -= 1;
					if (angle[1] <= 0)
						angle[1] = 360;
					pt[1] = getPosition(center[1], radius, angle[1]);
				}
			}

			else if (valueRectangle[1] % 2 == 1)
			{
				if (inputC[1] % 2 == 0) {
					if (rectangleCheck[1] % 2 == 0)
					{
						if (rectangleMinus[1] % 2 == 0) {
							rectX[1] += 5;
							if (rectX[1] >= 550) {
								rectangleMinus[1]++;
								rectangleCheck[1]++;
							}
						}
						else if (rectangleMinus[1] % 2 == 1) {
							rectX[1] -= 5;
							if (rectX[1] <= 350) {
								rectangleMinus[1]++;
								rectangleCheck[1]++;
							}
						}
					}
					else if (rectangleCheck[1] % 2 == 1) {
						if (rectangleYminus[1] % 2 == 0)
						{
							rectY[1] += 5;
							if (rectY[1] >= 250) {
								rectangleYminus[1]++;
								rectangleCheck[1]++;
							}
						}
						else if (rectangleYminus[1] % 2 == 1) {
							rectY[1] -= 5;
							if (rectY[1] <= 50) {
								rectangleYminus[1]++;
								rectangleCheck[1]++;
							}

						}
					}
				}
				else if (inputC[1] % 2 == 1)
				{
					if (rectangleCheck[1] % 2 == 0) {
						if (rectangleYminus[1] % 2 == 0) {
							rectY[1] += 5;
							if (rectY[1] >= 250) {
								rectangleYminus[1]++;
								rectangleCheck[1]++;
							}
						}
						else if (rectangleYminus[1] % 2 == 1) {
							rectY[1] -= 5;
							if (rectY[1] <= 50) {
								rectangleYminus[1]++;
								rectangleCheck[1]++;
							}
						}
					}
					else if (rectangleCheck[1] % 2 == 1) {
						if (rectangleMinus[1] % 2 == 0) {
							rectX[1] += 5;
							if (rectX[1] >= 550) {
								rectangleMinus[1]++;
								rectangleCheck[1]++;
							}
						}
						else if (rectangleMinus[1] % 2 == 1) {
							rectX[1] -= 5;
							if (rectX[1] <= 350) {
								rectangleMinus[1]++;
								rectangleCheck[1]++;
							}
						}

					}
				}
			}
			else if (valueTriangle[1] % 2 == 1) {
				if (inputC[1] % 2 == 0) {
					if (inputC[1] % 2 == 0) {
						if (TriangleCheck[1] % 3 == 0)
						{
							rectTx[1] += 5;
							rectTy[1] -= 10;
							if (rectTy[1] <= 50)
								TriangleCheck[1]++;
						}
						else if (TriangleCheck[1] % 3 == 1)
						{
							rectTx[1] += 5;
							rectTy[1] += 10;
							if (rectTy[1] >= 250)
								TriangleCheck[1]++;
						}
						else if (TriangleCheck[1] % 3 == 2)
						{
							rectTx[1] -= 5;
							if (rectTx[1] <= 350)
								TriangleCheck[1]++;
						}
					}
				}

				else if (inputC[1] % 2 == 1) {
					if (TriangleCheck[1] % 3 == 0)
					{
						rectTx[1] += 5;
						if (rectTx[1] >= 250)
							TriangleCheck[1]++;
					}
					else if (TriangleCheck[1] % 3 == 1)
					{
						rectTx[1] -= 5;
						rectTy[1] -= 10;
						if (rectTy[1] <= 50)
							TriangleCheck[1]++;
					}
					else if (TriangleCheck[1] % 3 == 2)
					{
						rectTx[1] -= 5;
						rectTy[1] += 10;
						if (rectTy[1] >= 250)
							TriangleCheck[1]++;
					}
				}
			}
			break;

		case 3:
			if (valueRectangle[2] % 2 == 0 && valueTriangle[2] % 2 == 0) {
				if (inputC[2] % 2 == 0) {
					angle[2] += 1;
					if (angle[2] >= 360)
						angle[2] = 0;
					pt[2] = getPosition(center[2], radius, angle[2]);
				}
				else if (inputC[2] % 2 == 1) {
					angle[2] -= 1;
					if (angle[2] <= 0)
						angle[2] = 360;
					pt[2] = getPosition(center[2], radius, angle[2]);
				}
			}
			else if (valueRectangle[2] % 2 == 1)
			{
				if (inputC[2] % 2 == 0) {
					if (rectangleCheck[2] % 2 == 0)
					{
						if (rectangleMinus[2] % 2 == 0) {
							rectX[2] += 5;
							if (rectX[2] >= 250) {
								rectangleMinus[2]++;
								rectangleCheck[2]++;
							}
						}
						else if (rectangleMinus[2] % 2 == 1) {
							rectX[2] -= 5;
							if (rectX[2] <= 50) {
								rectangleMinus[2]++;
								rectangleCheck[2]++;
							}
						}
					}
					else if (rectangleCheck[2] % 2 == 1) {
						if (rectangleYminus[2] % 2 == 0)
						{
							rectY[2] += 5;
							if (rectY[2] >= 550) {
								rectangleYminus[2]++;
								rectangleCheck[2]++;
							}
						}
						else if (rectangleYminus[2] % 2 == 1) {
							rectY[2] -= 5;
							if (rectY[2] <= 350) {
								rectangleYminus[2]++;
								rectangleCheck[2]++;
							}

						}
					}
				}
				else if (inputC[2] % 2 == 1)
				{
					if (rectangleCheck[2] % 2 == 0) {
						if (rectangleYminus[2] % 2 == 0) {
							rectY[2] += 5;
							if (rectY[2] >= 550) {
								rectangleYminus[2]++;
								rectangleCheck[2]++;
							}
						}
						else if (rectangleYminus[2] % 2 == 1) {
							rectY[2] -= 5;
							if (rectY[2] <= 350) {
								rectangleYminus[2]++;
								rectangleCheck[2]++;
							}
						}
					}
					else if (rectangleCheck[2] % 2 == 1) {
						if (rectangleMinus[2] % 2 == 0) {
							rectX[2] += 5;
							if (rectX[2] >= 250) {
								rectangleMinus[2]++;
								rectangleCheck[2]++;
							}
						}
						else if (rectangleMinus[2] % 2 == 1) {
							rectX[2] -= 5;
							if (rectX[2] <= 50) {
								rectangleMinus[2]++;
								rectangleCheck[2]++;
							}
						}

					}
				}
			}
			else if (valueTriangle[2] % 2 == 1) {
				if (inputC[2] % 2 == 0) {
					if (TriangleCheck[2] % 3 == 0)
					{
						rectTx[2] += 5;
						rectTy[2] -= 10;
						if (rectTy[2] <= 350)
							TriangleCheck[2]++;
					}
					else if (TriangleCheck[2] % 3 == 1)
					{
						rectTx[2] += 5;
						rectTy[2] += 10;
						if (rectTy[2] >= 550)
							TriangleCheck[2]++;
					}
					else if (TriangleCheck[2] % 3 == 2)
					{
						rectTx[2] -= 5;
						if (rectTx[2] <= 50)
							TriangleCheck[2]++;
					}
				}

				else if (inputC[2] % 2 == 1) {
					if (TriangleCheck[2] % 3 == 0)
					{
						rectTx[2] += 5;
						if (rectTx[2] >= 550)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[2] % 3 == 1)
					{
						rectTx[2] -= 5;
						rectTy[2] -= 10;
						if (rectTy[2] <= 350)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[3] % 3 == 2)
					{
						rectTx[2] -= 5;
						rectTy[2] += 10;
						if (rectTy[2] >= 550)
							TriangleCheck[2]++;
					}
				}
			}
			break;

		case 4:
			if (valueRectangle[3] % 2 == 0 && valueTriangle[3] % 2 == 0) {
				if (inputC[3] % 2 == 0) {
					angle[3] += 1;
					if (angle[3] >= 360)
						angle[3] = 0;
					pt[3] = getPosition(center[3], radius, angle[3]);
				}
				else if (inputC[3] % 2 == 1) {
					angle[3] -= 1;
					if (angle[3] <= 0)
						angle[3] = 360;
					pt[3] = getPosition(center[3], radius, angle[3]);
				}
			}
			else if (valueRectangle[3] % 2 == 1)
			{
				if (inputC[3] % 2 == 0) {
					if (rectangleCheck[3] % 2 == 0)
					{
						if (rectangleMinus[3] % 2 == 0) {
							rectX[3] += 5;
							if (rectX[3] >= 550) {
								rectangleMinus[3]++;
								rectangleCheck[3]++;
							}
						}
						else if (rectangleMinus[3] % 2 == 1) {
							rectX[3] -= 5;
							if (rectX[3] <= 350) {
								rectangleMinus[3]++;
								rectangleCheck[3]++;
							}
						}
					}
					else if (rectangleCheck[3] % 2 == 1) {
						if (rectangleYminus[3] % 2 == 0)
						{
							rectY[3] += 5;
							if (rectY[3] >= 550) {
								rectangleYminus[3]++;
								rectangleCheck[3]++;
							}
						}
						else if (rectangleYminus[3] % 2 == 1) {
							rectY[3] -= 5;
							if (rectY[3] <= 350) {
								rectangleYminus[3]++;
								rectangleCheck[3]++;
							}

						}
					}
				}
				else if (inputC[3] % 2 == 1)
				{
					if (rectangleCheck[3] % 2 == 0) {
						if (rectangleYminus[3] % 2 == 0) {
							rectY[3] += 5;
							if (rectY[3] >= 550) {
								rectangleYminus[3]++;
								rectangleCheck[3]++;
							}
						}
						else if (rectangleYminus[3] % 2 == 1) {
							rectY[3] -= 5;
							if (rectY[3] <= 350) {
								rectangleYminus[3]++;
								rectangleCheck[3]++;
							}
						}
					}
					else if (rectangleCheck[3] % 2 == 1) {
						if (rectangleMinus[3] % 2 == 0) {
							rectX[3] += 5;
							if (rectX[3] >= 550) {
								rectangleMinus[3]++;
								rectangleCheck[3]++;
							}
						}
						else if (rectangleMinus[3] % 2 == 1) {
							rectX[3] -= 5;
							if (rectX[3] <= 350) {
								rectangleMinus[3]++;
								rectangleCheck[3]++;
							}
						}

					}
				}
			}

			else if (valueTriangle[3] % 2 == 1) {
				if (inputC[3] % 2 == 0) {
					if (TriangleCheck[3] % 3 == 0)
					{
						rectTx[3] += 5;
						rectTy[3] -= 10;
						if (rectTy[3] <= 350)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[3] % 3 == 1)
					{
						rectTx[3] += 5;
						rectTy[3] += 10;
						if (rectTy[3] >= 550)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[3] % 3 == 2)
					{
						rectTx[3] -= 5;
						if (rectTx[3] <= 350)
							TriangleCheck[3]++;
					}
				}
				else if (inputC[3] % 2 == 1) {
					if (TriangleCheck[3] % 3 == 0)
					{
						rectTx[3] += 5;
						if (rectTx[3] >= 550)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[3] % 3 == 1)
					{
						rectTx[3] -= 5;
						rectTy[3] -= 10;
						if (rectTy[3] <= 350)
							TriangleCheck[3]++;
					}
					else if (TriangleCheck[3] % 3 == 2)
					{
						rectTx[3] -= 5;
						rectTy[3] += 10;
						if (rectTy[3] >= 550)
							TriangleCheck[3]++;
					}
				}

			}
			break;
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (value == 1) {
			if (set == 0) {
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 0, 0, 300, 300);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 0, 0, 300, 300);
			}
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (valueRectangle[0] % 2 == 0 && valueTriangle[0] % 2 == 0) {
				Ellipse(hdc, center[0].x - 100, center[0].y - 100, center[0].x + 100, center[0].y + 100);
			}
			else if (valueRectangle[0] % 2 == 1)
				Rectangle(hdc, center[0].x - 100, center[0].y - 100, center[0].x + 100, center[0].y + 100);
			else if (valueTriangle[0] % 2 == 1)
			{
				Triangle[0].triangle[0].x = center[0].x - 100;
				Triangle[0].triangle[0].y = center[0].y + 100;
				Triangle[0].triangle[1].x = center[0].x + 100;
				Triangle[0].triangle[1].y = center[0].y + 100;
				Triangle[0].triangle[2].x = center[0].x;
				Triangle[0].triangle[2].y = center[0].y - 100;
				Polygon(hdc, Triangle[0].triangle, 3);

			}

			if (valueCircle[0] % 2 == 0) {
				if (valueRectangle[0] % 2 == 0 && valueTriangle[0] % 2 == 0)
					Ellipse(hdc, pt[0].x - 5, pt[0].y - 5, pt[0].x + 5, pt[0].y + 5);//원그리기

				else if (valueRectangle[0] % 2 == 1)
					Ellipse(hdc, rectX[0] - 5, rectY[0] - 5, rectX[0] + 5, rectY[0] + 5);
				else if (valueTriangle[0] % 2 == 1)
					Ellipse(hdc, rectTx[0] - 5, rectTy[0] - 5, rectTx[0] + 5, rectTy[0] + 5);
			}


			else if (valueCircle[0] % 2 == 1) {
				if (valueRectangle[0] % 2 == 1)
					Rectangle(hdc, rectX[0] - 5, rectY[0] - 5, rectX[0] + 5, rectY[0] + 5);
				else if (valueRectangle[0] % 2 == 0 && valueTriangle[0] % 2 == 0)
					Rectangle(hdc, pt[0].x - 5, pt[0].y - 5, pt[0].x + 5, pt[0].y + 5);
				else if (valueTriangle[0] % 2 == 1)
					Rectangle(hdc, rectTx[0] - 5, rectTy[0] - 5, rectTx[0] + 5, rectTy[0] + 5);
			}

			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, center[0].x - 5, center[0].y - 5, center[0].x + 5, center[0].y + 5);


			if (set == 1) {
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 300, 0, 600, 300);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 300, 0, 600, 300);
			}

			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (valueRectangle[1] % 2 == 0 && valueTriangle[1] % 2 == 0)
				Ellipse(hdc, center[1].x - 100, center[1].y - 100, center[1].x + 100, center[1].y + 100);
			else if (valueRectangle[1] % 2 == 1)
				Rectangle(hdc, center[1].x - 100, center[1].y - 100, center[1].x + 100, center[1].y + 100);
			else if (valueTriangle[1] % 2 == 1)
			{
				Triangle[1].triangle[0].x = center[1].x - 100;
				Triangle[1].triangle[0].y = center[1].y + 100;
				Triangle[1].triangle[1].x = center[1].x + 100;
				Triangle[1].triangle[1].y = center[1].y + 100;
				Triangle[1].triangle[2].x = center[1].x;
				Triangle[1].triangle[2].y = center[1].y - 100;
				Polygon(hdc, Triangle[1].triangle, 3);

			}

			if (valueCircle[1] % 2 == 0) {
				if (valueRectangle[1] % 2 == 0 && valueTriangle[1] % 2 == 0)
					Ellipse(hdc, pt[1].x - 5, pt[1].y - 5, pt[1].x + 5, pt[1].y + 5);//원그리기

				else if (valueRectangle[1] % 2 == 1)
					Ellipse(hdc, rectX[1] - 5, rectY[1] - 5, rectX[1] + 5, rectY[1] + 5);
				else if (valueTriangle[1] % 2 == 1)
					Ellipse(hdc, rectTx[1] - 5, rectTy[1] - 5, rectTx[1] + 5, rectTy[1] + 5);
			}


			else if (valueCircle[1] % 2 == 1) {
				if (valueRectangle[1] % 2 == 1)
					Rectangle(hdc, rectX[1] - 5, rectY[1] - 5, rectX[1] + 5, rectY[1] + 5);
				else if (valueRectangle[0] % 2 == 0 && valueTriangle[1] % 2 == 0)
					Rectangle(hdc, pt[1].x - 5, pt[1].y - 5, pt[1].x + 5, pt[1].y + 5);
				else if (valueTriangle[1] % 2 == 1)
					Rectangle(hdc, rectTx[1] - 5, rectTy[1] - 5, rectTx[1] + 5, rectTy[1] + 5);
			}

			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, center[1].x - 5, center[1].y - 5, center[1].x + 5, center[1].y + 5);

			if (set == 2) {
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 0, 300, 300, 600);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 0, 300, 300, 600);
			}
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (valueRectangle[2] % 2 == 0 && valueTriangle[2] % 2 == 0)
				Ellipse(hdc, center[2].x - 100, center[2].y - 100, center[2].x + 100, center[2].y + 100);
			else if (valueRectangle[2] % 2 == 1)
				Rectangle(hdc, center[2].x - 100, center[2].y - 100, center[2].x + 100, center[2].y + 100);
			else if (valueTriangle[2] % 2 == 1)
			{
				Triangle[2].triangle[0].x = center[2].x - 100;
				Triangle[2].triangle[0].y = center[2].y + 100;
				Triangle[2].triangle[1].x = center[2].x + 100;
				Triangle[2].triangle[1].y = center[2].y + 100;
				Triangle[2].triangle[2].x = center[2].x;
				Triangle[2].triangle[2].y = center[2].y - 100;
				Polygon(hdc, Triangle[2].triangle, 3);

			}

			if (valueCircle[2] % 2 == 0) {
				if (valueRectangle[2] % 2 == 0 && valueTriangle[2] % 2 == 0)
					Ellipse(hdc, pt[2].x - 5, pt[2].y - 5, pt[2].x + 5, pt[2].y + 5);//원그리기

				else if (valueRectangle[2] % 2 == 1)
					Ellipse(hdc, rectX[2] - 5, rectY[2] - 5, rectX[2] + 5, rectY[2] + 5);
				else if (valueTriangle[2] % 2 == 1)
					Ellipse(hdc, rectTx[2] - 5, rectTy[2] - 5, rectTx[2] + 5, rectTy[2] + 5);
			}


			else if (valueCircle[2] % 2 == 1) {
				if (valueRectangle[2] % 2 == 1)
					Rectangle(hdc, rectX[2] - 5, rectY[2] - 5, rectX[2] + 5, rectY[2] + 5);
				else if (valueRectangle[2] % 2 == 0 && valueTriangle[2] % 2 == 0)
					Rectangle(hdc, pt[2].x - 5, pt[2].y - 5, pt[2].x + 5, pt[2].y + 5);
				else if (valueTriangle[2] % 2 == 1)
					Rectangle(hdc, rectTx[2] - 5, rectTy[2] - 5, rectTx[2] + 5, rectTy[2] + 5);
			}

			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, center[2].x - 5, center[2].y - 5, center[2].x + 5, center[2].y + 5);

			if (set == 3) {
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 300, 300, 600, 600);
			}
			else {
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, 300, 300, 600, 600);
			}
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);

			if (valueRectangle[3] % 2 == 0 && valueTriangle[3] % 2 == 0)
				Ellipse(hdc, center[3].x - 100, center[3].y - 100, center[3].x + 100, center[3].y + 100);
			else if (valueRectangle[3] % 2 == 1) {
				Rectangle(hdc, center[3].x - 100, center[3].y - 100, center[3].x + 100, center[3].y + 100);

			}
			else if (valueTriangle[3] % 2 == 1)
			{
				Triangle[3].triangle[0].x = center[3].x - 100;
				Triangle[3].triangle[0].y = center[3].y + 100;
				Triangle[3].triangle[1].x = center[3].x + 100;
				Triangle[3].triangle[1].y = center[3].y + 100;
				Triangle[3].triangle[2].x = center[3].x;
				Triangle[3].triangle[2].y = center[3].y - 100;
				Polygon(hdc, Triangle[3].triangle, 3);
			}


			if (valueCircle[3] % 2 == 0) {
				if (valueRectangle[3] % 2 == 0 && valueTriangle[3] % 2 == 0)
					Ellipse(hdc, pt[3].x - 5, pt[3].y - 5, pt[3].x + 5, pt[3].y + 5);//원그리기

				else if (valueRectangle[3] % 2 == 1)
					Ellipse(hdc, rectX[3] - 5, rectY[3] - 5, rectX[3] + 5, rectY[3] + 5);

				else if (valueTriangle[3] % 2 == 1)
					Ellipse(hdc, rectTx[3] - 5, rectTy[3] - 5, rectTx[3] + 5, rectTy[3] + 5);
			}


			else if (valueCircle[3] % 2 == 1) {
				if (valueRectangle[3] % 2 == 1)
					Rectangle(hdc, rectX[3] - 5, rectY[3] - 5, rectX[3] + 5, rectY[3] + 5);
				else if (valueRectangle[3] % 2 == 0 && valueTriangle[3] % 2 == 0)
					Rectangle(hdc, pt[3].x - 5, pt[3].y - 5, pt[3].x + 5, pt[3].y + 5);
				else if (valueTriangle[3] % 2 == 1)
					Rectangle(hdc, rectTx[3] - 5, rectTy[3] - 5, rectTx[3] + 5, rectTy[3] + 5);
			}

			hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, center[3].x - 5, center[3].y - 5, center[3].x + 5, center[3].y + 5);
		}

		else if (value == 2)
		{

		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
