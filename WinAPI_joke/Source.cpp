#include <windows.h> 
#include <math.h>

#define captionOld "Ты не сможешь выбрать \"Нет\"" //"Ты идиот?"
#define captionNew "Справедливо, теперь можно и выйти"

#define speedButton 50                      // Скорость кнопки
#define hitButton 10                        // Количество пикселей, на которое мышь может наезжать на кнопку
#define coloreBackground RGB(0, 0, 0)
#define coloreText RGB(255, 255, 255)
#define nVisible 230                        // Прозрачность
#define sizeWindowW 600
#define sizeWindowH 300
#define sizeButtonW 100
#define sizeButtonH 50

#define window_id 0
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

int masButtonPos[][2] = { {350, 170}, { 150, 170}, { 250, 170} }; 

BOOL MoveButton(int xPos, int yPos);
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
        CW_USEDEFAULT, NULL, sizeWindowW, sizeWindowH,
        (HWND)NULL, (HMENU)window_id, HINSTANCE(hInst), NULL);
    SetLayeredWindowAttributes(hwnd, 0, nVisible, LWA_ALPHA);
    ShowScrollBar(hwnd, SB_BOTH, false);

    hFontButton = CreateFont(20, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, NULL);

    hFontCaption = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEVICE_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, NULL);
   
    button1 = CreateWindow( L"BUTTON", L"Да",
        WS_CHILD | WS_VISIBLE,
        masButtonPos[1][0], masButtonPos[1][1], sizeButtonW, sizeButtonH,
        hwnd, (HMENU)button1_id, hThisInstance, NULL);
    SendMessage(button1, WM_SETFONT, (WPARAM)hFontButton, 0);

    caption1 = CreateWindow(L"STATIC", L" ", 
        WS_CHILD | WS_VISIBLE | ES_CENTER | SS_CENTERIMAGE,
        10, 10, sizeWindowW - 20 - 17, sizeWindowH / 2 - 10, //563, 130
        hwnd, (HMENU)caption1_id, hThisInstance, NULL);
    SendMessage(caption1, WM_SETFONT, (WPARAM)hFontCaption, 0);
    SetWindowTextA(caption1, captionOld);

    button2 = CreateWindow(L"BUTTON", L"Нет",
        WS_CHILD | WS_VISIBLE | WS_EX_TOPMOST,
        masButtonPos[0][0], masButtonPos[0][1], sizeButtonW, sizeButtonH,
        hwnd, (HMENU)button2_id, hThisInstance, NULL);
    SendMessage(button2, WM_SETFONT, (WPARAM)hFontButton, TRUE);
   
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    //SetWindowTextA(caption1, captionOld);
    
    BOOL changeButton;
    LPPOINT pPnt = (LPPOINT)malloc(sizeof(*pPnt));
    
    while (GetMessage(&msg, NULL, NULL, NULL)) 
    {
        GetCursorPos(pPnt);
        ScreenToClient(hwnd, pPnt);
        changeButton = MoveButton(pPnt[0].x, pPnt[0].y);
        if (changeButton)
        {
            MoveWindow(button2, masButtonPos[0][0], masButtonPos[0][1], sizeButtonW, sizeButtonH, TRUE);
        }
       
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
        //SetBkColor(hdcStatic, coloreText);
        SetBkMode((HDC)wParam, TRANSPARENT);
        //return (INT_PTR)CreateSolidBrush(coloreBackground);
        return (INT_PTR)CreateSolidBrush(NULL_BRUSH);
        //return (INT_PTR)CreateSolidBrush(WHITE_BRUSH);
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
        UpdateWindow(hWnd);
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
            SetWindowTextA(caption1, captionNew);
            button3 = CreateWindow(L"BUTTON", L"Выход",
                WS_CHILD | WS_VISIBLE | WS_EX_TOPMOST,
                masButtonPos[2][0], masButtonPos[2][1], sizeButtonW, sizeButtonH,
                hwnd, (HMENU)button3_id, hThisInstance, NULL);
            SendMessage(button3, WM_SETFONT, (WPARAM)hFontButton, TRUE);
        }

        if (wParam == button2_id)
        {
            MoveButton(masButtonPos[0][0], masButtonPos[0][1]);
            MoveWindow(button2, masButtonPos[0][0], masButtonPos[0][1], 100, 50, TRUE);
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

BOOL MoveButton(int xPos, int yPos)
{
    BOOL flag = FALSE;
    int x = masButtonPos[0][0] + 50;
    int y = masButtonPos[0][1] + 25;

    if (abs(xPos - x) <= 50 + hitButton && abs(yPos - y) <= 25 + hitButton)
    {
        int dx = x - xPos;
        int dy = y - yPos;
        float dxy = sqrt(dx * dx + dy * dy);
        int kdx = speedButton * abs(dx) / (int)dxy;
        int kdy = speedButton * abs(dy) / (int)dxy;

        masButtonPos[0][0] = (dx <= 0) ? masButtonPos[0][0] - kdx : masButtonPos[0][0] + kdx;
        masButtonPos[0][1] = (dy <= 0) ? masButtonPos[0][1] - kdy : masButtonPos[0][1] + kdy;

        //Упрощенный вариант без скорости
        /*masButtonPos[0][0] = masButtonPos[0][0] + dx;
        masButtonPos[0][1] = masButtonPos[0][1] + dy;*/

        flag = TRUE;
    }

    if (masButtonPos[0][0] < 0 || masButtonPos[0][1] < 0 || masButtonPos[0][0] + 100 > rct.right || masButtonPos[0][1] + 50 > rct.bottom)
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

        masButtonPos[0][0] = masButtonPos[0][0] + xSign * (dx / 2);
        masButtonPos[0][1] = masButtonPos[0][1] + ySign * (dy / 2);

        flag = TRUE;
    }
        
    return flag;
}
