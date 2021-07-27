#include <windows.h> 
#include <math.h>

#define speedButton 50                      // Скорость кнопки
#define hitButton 10                        // Количество пикселей, на которое мышь может наезжать на кнопку
#define coloreBackground RGB(0, 0, 0)
#define coloreText RGB(255, 255, 255)
#define nVisible 230                        // Прозрачность

#define button1_id 1
#define button2_id 2
#define button3_id 3
#define caption1_id 4

HWND hwnd;
HWND button1;
HWND button2;
HWND button3;
HWND caption1;

HFONT hFontButton;
HFONT hFontCaption;

RECT rct;
HINSTANCE hThisInstance;

int masButtonPos[] = { 350, 150 };

void MoveButton(int xPos, int yPos);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    hThisInstance = hInst;
    MSG msg;

    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW ;
    wc.lpfnWndProc = WndProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"Window";
    wc.cbWndExtra = NULL;
    wc.cbClsExtra = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(coloreBackground);;
    wc.hInstance = hInst;
    
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; 
    }

    hwnd = CreateWindowEx(WS_EX_LAYERED, L"Window", L"Опрос",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        CW_USEDEFAULT, NULL, 600, 300,
        (HWND)NULL, NULL, HINSTANCE(hInst), NULL);
    SetLayeredWindowAttributes(hwnd, 0, nVisible, LWA_ALPHA);

    /*hwnd = CreateWindow(L"Window", L"Опрос",
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
        CW_USEDEFAULT, NULL, 600, 300,
        (HWND)NULL, NULL, HINSTANCE(hInst), NULL);
    
    if (!hwnd) {
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }*/

    ShowScrollBar(hwnd, SB_BOTH, false);

    hFontButton = CreateFont(20, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, NULL);

    hFontCaption = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, NULL);
   
    button1 = CreateWindow( L"BUTTON", L"Да",
        WS_CHILD | WS_VISIBLE,
        masButtonPos[0] - 200, masButtonPos[1], 100, 50,
        hwnd, (HMENU)button1_id, hThisInstance, NULL);
    SendMessage(button1, WM_SETFONT, (WPARAM)hFontButton, 0);

    caption1 = CreateWindow(L"STATIC", L"Ты идиот?", //Ты не сможешь выбрать \"Нет\"
        WS_CHILD | WS_VISIBLE | ES_CENTER | SS_CENTERIMAGE,
        10, 10, 563, 130,
        hwnd, (HMENU)caption1_id, hThisInstance, NULL);
    SendMessage(caption1, WM_SETFONT, (WPARAM)hFontCaption, 0);
    //SetTextColor(GetDC(caption1), coloreText);
    //SetBkColor(GetDC(caption1), coloreBackground);
    //UpdateWindow(caption1);
    //CreateSolidBrush(RGB(0, 0, 0));
    //SendMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)caption1, 0);
    SendMessage(caption1, WM_CTLCOLORSTATIC, (WPARAM)caption1, (LPARAM)GetDlgItem(hwnd, caption1_id));

    button2 = CreateWindow(L"BUTTON", L"Нет",
        WS_CHILD | WS_VISIBLE | WS_EX_TOPMOST,
        masButtonPos[0], masButtonPos[1], 100, 50,
        hwnd, (HMENU)button2_id, hThisInstance, NULL);
    SendMessage(button2, WM_SETFONT, (WPARAM)hFontButton, TRUE);
   
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    LPPOINT pPnt = (LPPOINT)malloc(sizeof(*pPnt));
    while (GetMessage(&msg, NULL, NULL, NULL)) 
    {
        GetCursorPos(pPnt);
        ScreenToClient(hwnd, pPnt);
        MoveButton(pPnt[0].x, pPnt[0].y);
        MoveWindow(button2, masButtonPos[0], masButtonPos[1], 100, 50, TRUE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    free(pPnt);
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, coloreText);
        SetBkColor(hdcStatic, coloreBackground);
        return (INT_PTR)CreateSolidBrush(coloreBackground);
    }

    case WM_CREATE:
        GetClientRect(hwnd, &rct);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(NULL);
        break;

    case WM_CLOSE:
        break;

    case WM_PAINT:
        UpdateWindow(hwnd);
        break;

    case WM_SIZE:
        GetClientRect(hwnd, &rct);
        break;

    case WM_COMMAND:
        if (wParam == button1_id)
        {
            SendMessage(button1, WM_CLOSE, 0, 0);
            DestroyWindow(button1);
            SendMessage(button2, WM_CLOSE, 0, 0);
            DestroyWindow(button2);
            UpdateWindow(hwnd);
            SetWindowTextA(caption1, "Справедливо, теперь можно и выйти");
            button3 = CreateWindow(L"BUTTON", L"Выход",
                WS_CHILD | WS_VISIBLE | WS_EX_TOPMOST,
                250, 150, 100, 50,
                hwnd, (HMENU)button3_id, hThisInstance, NULL);
            SendMessage(button3, WM_SETFONT, (WPARAM)hFontButton, TRUE);
        }

        if (wParam == button2_id)
        {
            MoveButton(masButtonPos[0], masButtonPos[1]);
            MoveWindow(button2, masButtonPos[0], masButtonPos[1], 100, 50, TRUE);
        }

        if (wParam == button3_id)
        {
            PostQuitMessage(NULL);
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

    if (abs(xPos - x) <= 50 + hitButton && abs(yPos - y) <= 25 + hitButton)
    {
        int dx = x - xPos;
        int dy = y - yPos;
        float dxy = sqrt(dx * dx + dy * dy);
        int kdx = speedButton * abs(dx) % (int)dxy;
        int kdy = speedButton * abs(dy) % (int)dxy;

        masButtonPos[0] = (dx <= 0) ? masButtonPos[0] - kdx: masButtonPos[0] + kdx;
        masButtonPos[1] = (dy <= 0) ? masButtonPos[1] - kdy : masButtonPos[1] + kdy;      
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