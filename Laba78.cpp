#include <Windows.h>
#include <tchar.h>
#include <math.h>
#include <time.h>

#define CLASS_NAME L"FlowerWindowClass"
#define BTN_WIDTH 150
#define BTN_HEIGHT 30

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int action = 0;
int flowerX = 200;
int flowerY = 300;
int prevX = flowerX;
int prevY = flowerY;
bool isMoving = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"Flower", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nShowCmd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

int GetClientRectWidth(HWND hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);
	return rc.right;
}

//для лепестков
void DrawPetal(HDC hdc, int cntrX, int cntrY, double a, double b, double theta) {
	double x_c = a * cos(theta);
	double y_c = a * sin(theta);

	//центр эллипса в координатах окна
	int cx = cntrX + (int)(x_c + 0, 5);
	int cy = cntrY + (int)(y_c + 0, 5);

	const int nPoints = 100;
	POINT pts[nPoints];
	for (int i = 0; i < nPoints; ++i)
	{
		double t = 2.0 * 3.14 * i / nPoints;
		double x = x_c + a * cos(t) * cos(theta) - b * sin(t) * sin(theta);
		double y = y_c + a * cos(t) * sin(theta) + b * sin(t) * cos(theta);

		pts[i].x = cntrX + (int)(x + 0.5);
		pts[i].y = cntrY - (int)(y + 0.5);
	}
	Polygon(hdc, pts, nPoints);
}

void DrawFlower(HDC hdc, HWND hWnd) {
	PAINTSTRUCT ps;
	RECT rect;
	GetClientRect(hWnd, &rect);

	HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, hBackgroundBrush);
	DeleteObject(hBackgroundBrush);

	//стебель
	HBRUSH hGreen = CreateSolidBrush(RGB(0, 200, 0));
	HPEN hGreenPen = CreatePen(PS_SOLID, 10, RGB(0, 128, 0));
	SelectObject(hdc, hGreenPen);
	SelectObject(hdc, hGreen);
	Rectangle(hdc, flowerX - 5, flowerY, flowerX + 5, flowerY + 500);


	double a = 120; //большая полуось
	double b = 60;  //малая полуось
	//лепестки
	HBRUSH hBlue = CreateSolidBrush(RGB(0, 102, 255));
	HPEN hBluePen = CreatePen(PS_SOLID, 2, RGB(0, 49, 82));
	SelectObject(hdc, hBlue);
	SelectObject(hdc, hBluePen);
	for (int i = 0; i < 16; ++i)
	{
		double theta = 2 * 3.14 * i / 16;
		DrawPetal(hdc, flowerX, flowerY, a, b, theta);
	}
	DeleteObject(hBlue);
	DeleteObject(hBluePen);

	//середина
	HBRUSH hYellow = CreateSolidBrush(RGB(255, 255, 0));
	HPEN hYellowPen = CreatePen(PS_SOLID, 2, RGB(200, 200, 0));
	SelectObject(hdc, hYellowPen);
	SelectObject(hdc, hYellow);
	Ellipse(hdc, flowerX - 35, flowerY - 35, flowerX + 35, flowerY + 35);

	DeleteObject(hGreen);    DeleteObject(hGreenPen);
	DeleteObject(hYellow);   DeleteObject(hYellowPen);
}

//оконная функция вызывается операционной системой и получает сообщения из очереди для данного приложения
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HDC hdc;
	PAINTSTRUCT ps;
	static int timerID = 1;

	switch (message) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 100, NULL);
		CreateWindowW(L"button", L"Показать Рисунок", WS_CHILD |
			WS_VISIBLE | BS_PUSHBUTTON, 50, 10, BTN_WIDTH,
			BTN_HEIGHT, hWnd, (HMENU)1, NULL, NULL);
		CreateWindowW(L"button", L"Очистить", WS_CHILD |
			WS_VISIBLE | BS_PUSHBUTTON, 250, 10, BTN_WIDTH,
			BTN_HEIGHT, hWnd, (HMENU)2, NULL, NULL);
		CreateWindowW(L"button", L"ФИО автора", WS_CHILD |
			WS_VISIBLE | BS_PUSHBUTTON, 450, 10, BTN_WIDTH,
			BTN_HEIGHT, hWnd, (HMENU)3, NULL, NULL);
		return 0;

	case WM_LBUTTONDOWN:
		isMoving = true;
		SetTimer(hWnd, timerID, 30, NULL);
		return 0;

	case WM_RBUTTONDOWN:
		isMoving = false;
		KillTimer(hWnd, timerID);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1:
			action = 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			action = 2;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 3:
			action = 3;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (action == 1) {
			DrawFlower(hdc, hWnd);
		}
		else if (action == 2) {
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		}
		else if (action == 3) {
			TextOut(hdc, 350, 250, L"Бирюкова С.В.", wcslen(L"Бирюкова С.В."));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		prevX = flowerX;
		prevY = flowerY;

		flowerX += 5;
		if (flowerX > GetClientRectWidth(hWnd)) {
			flowerX = 0;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, timerID);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}
