#include <windows.h> 
#include <math.h>

#define button1_id 1
#define button2_id 2

HWND hwnd;
HWND button1;
HWND button2;

RECT rct;

int masButtonPos[] = { 370, 330 };

HINSTANCE hThisInstance;

void MoveButton(int xPos, int yPos);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    hThisInstance = hInst;
    MSG msg;

    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Window";
    wc.cbWndExtra = NULL;
    wc.cbClsExtra = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hInstance = hInst;
    
    // Проверка существования класса
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; 
    }

    // Функция, создающая окно   
    hwnd = CreateWindow(L"Window", L"Окно",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
        CW_USEDEFAULT, NULL, 640, 480,
        (HWND)NULL, NULL, HINSTANCE(hInst), NULL);
    
    button1 = CreateWindow( L"BUTTON", L"Да",
        WS_CHILD | WS_VISIBLE,
        170, 330, 100, 50,
        hwnd, (HMENU)button1_id, hThisInstance, NULL);

    button2 = CreateWindow( L"BUTTON", L"Нет",
        WS_CHILD | WS_VISIBLE,
        masButtonPos[0], masButtonPos[1], 100, 50,
        hwnd, (HMENU)button2_id, hThisInstance, NULL);

    // Проверка коррректности создания окна
    if (!hwnd) {
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, NULL, NULL)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    int xPos;
    int yPos;

    switch (uMsg) 
    {
    case WM_CREATE:
        GetClientRect(hwnd, &rct);
        return 0;

    case WM_PAINT:
        UpdateWindow(hwnd);
        break;

    case WM_SIZE:
        GetClientRect(hwnd, &rct);
        break;

    case WM_MOUSEMOVE:
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
        MoveButton(xPos, yPos);
        MoveWindow(button2, masButtonPos[0], masButtonPos[1], 100, 50, TRUE);
        break;
    
    case WM_DESTROY:
        PostQuitMessage(NULL); 
        break;
    
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); 
    }

    return NULL; 
}

void MoveButton(int xPos, int yPos)
{
    int x = masButtonPos[0] + 50;
    int y = masButtonPos[1] + 25;

    if (xPos >= masButtonPos[0] - 20 
        && xPos <= masButtonPos[0] + 100 + 20
        && yPos >= masButtonPos[1] - 20 
        && yPos <= masButtonPos[1] + 50 + 20)
    {
        int speed = 4;
        int dx = x - xPos;
        int dy = y - yPos;
        float dxy = sqrt(dx * dx + dy * dy);
        int kdx = speed * abs(dx) % (int)dxy;
        int kdy = speed * abs(dy) % (int)dxy;

        //кнопка от мышки
        masButtonPos[0] = (dx <= 0) ? masButtonPos[0] - kdx: masButtonPos[0] + kdx;
        masButtonPos[1] = (dy <= 0) ? masButtonPos[1] - kdy : masButtonPos[1] + kdy;

        //кнопка за мышкой
        /*masButtonPos[0] = (dx <= 0) ? masButtonPos[0] + kdx : masButtonPos[0] - kdx;
        masButtonPos[1] = (dy <= 0) ? masButtonPos[1] + kdy : masButtonPos[1] - kdy;*/        
    }

    if (xPos > masButtonPos[0]
        && xPos < masButtonPos[0] + 100
        && yPos > masButtonPos[1]
        && yPos < masButtonPos[1] + 50)
    {

    }
    
}