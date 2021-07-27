#include <windows.h> 
#include <math.h>

#define button1_id 1
#define button2_id 2
#define caption1_id 3

HWND hwnd;
HWND button1;
HWND button2;
HWND caption1;

RECT rct;

int masButtonPos[] = { 350, 150 };

HINSTANCE hThisInstance;

void MoveButton(int xPos, int yPos);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    hThisInstance = hInst;
    MSG msg;

    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW /*| CS_NOCLOSE*/;
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

    hwnd = CreateWindow(L"Window", L"Опрос",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
        CW_USEDEFAULT, NULL, 600, 300,
        (HWND)NULL, NULL, HINSTANCE(hInst), NULL);
    
    if (!hwnd) {
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }

    HFONT hf;
    hf = CreateFont(20, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, NULL);
    
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
   
    button1 = CreateWindow( L"BUTTON", L"Да",
        WS_CHILD | WS_VISIBLE,
        masButtonPos[0] - 200, masButtonPos[1], 100, 50,
        hwnd, (HMENU)button1_id, hThisInstance, NULL);
    SendMessage(button1, WM_SETFONT, WPARAM(hf), 0);

    caption1 = CreateWindow(L"STATIC", L"Ты не сможешь выбрать \"Нет\"",
        WS_CHILD | WS_VISIBLE,
        10, 10, 580, 130,
        hwnd, (HMENU)caption1_id, hThisInstance, NULL);

    button2 = CreateWindow(L"BUTTON", L"Нет",
        WS_CHILD | WS_VISIBLE | WS_EX_TOPMOST,
        masButtonPos[0], masButtonPos[1], 100, 50,
        hwnd, (HMENU)button2_id, hThisInstance, NULL);
    SendMessage(button2, WM_SETFONT, (WPARAM)hFont, TRUE);
   
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    LPPOINT pPnt = (LPPOINT)malloc(sizeof(*pPnt));
    while (GetMessage(&msg, NULL, NULL, NULL)) 
    {
        GetCursorPos(pPnt);
        ScreenToClient(hwnd, pPnt);
        MoveButton(pPnt[0].x, pPnt[0].y);
        MoveWindow(button2, masButtonPos[0], masButtonPos[1], 100, 50, TRUE);
        /*SetCapture(hwnd);*/
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    free(pPnt);
    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    int xPos;
    int yPos;

    int x;
    int y;

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
        break;

    case WM_DESTROY:
        PostQuitMessage(NULL); 
        break;
    
    case WM_COMMAND:
        if (wParam == button2_id)
        {
            
            /*MoveButton(masButtonPos[0], masButtonPos[1]);
            MoveWindow(button2, masButtonPos[0], masButtonPos[1], 100, 50, TRUE);*/
        }
        if (wParam == button1_id)
        {
            SendMessage(button2, WM_CLOSE, 0, 0);
            DestroyWindow(button2);
            SetWindowTextA(caption1, "Ну вот");
        }
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

    if (abs(xPos - x) <= 50 + 10 && abs(yPos - y) <= 25 + 10)
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

    if (masButtonPos[0] < 0 || masButtonPos[1] < 0 || masButtonPos[0] + 100 > rct.right || masButtonPos[1] + 50 > rct.bottom)
    {
        float maxDistance = 0;
        int xSign = 1, ySign = 1;
        int dx, dy, dxy;

        dx = abs(rct.right - x);

        if (abs(x) > dx)
        {
            dx = abs(x);
            xSign = -1;
        }

        dy = abs(rct.bottom - y);
        if (abs(y) > dy)
        {
            dy = abs(y);
            xSign = -1;
        }

        masButtonPos[0] = masButtonPos[0] + xSign * (dx / 2);
        masButtonPos[1] = masButtonPos[1] + ySign * (dy / 2);
    }
    
}