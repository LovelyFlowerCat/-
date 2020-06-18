// 小学生算数练习.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "小学生算数练习.h"
#include <cassert>
#include <string>
#include <vector>
#include <objbase.h>//gdiplus必须导入它
#include <gdiplus.h>
#include <Windows.h>
#include <time.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace std;
using namespace Gdiplus;

#define MAX_LOADSTRING 100
#define BACKGROUND 1000
#define LABEL1 1001
#define LABEL2 1002
#define LABEL3 1003
#define BUTTON1 2001
#define BUTTON2 2002
#define BUTTON3 2003
#define BUTTON4 2004
#define BUTTON5 2005
#define BUTTON_EXIT 2006
#define EDITTEXT 3001

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND MainForm;
HWND backGroundImage;
HWND label1;
HWND label2;
HWND label3;
HWND button1;
HWND button2;
HWND button3;
HWND button4;
HWND button5;
HWND button_exit;
HWND editText;
HFONT labelFont, btnFont, pointFont;
HBRUSH brush;
vector<HWND> userControls;
int startPositionX = 0;
int startPositionY = 0;
int windowWidth = 800;
int windowHeight = 600;
int* result = new int;
int mode = 0;
int correct = 0;
int wrong = 0;
ULONG_PTR gdiplusToken = 0;
enum class State
{
    CORRECT,
    INCORRECT,
    TIMER_ON,
    TIMER_OFF
};
State answerState, timerState;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LPCWSTR             StringToLPCWSTR(string orig);
char*               LPWSTRToCharArray(LPWSTR lpString);
int                 GetBits(int number);
int                 random(int min, int max);
void CALLBACK       Timer_Tick(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
void                CenterDisplay(vector<HWND> controls);
void                MoveToCenter(HWND hwnd, int horizontal = 1, int vertical = 0, int hoffset = 0, int voffset = 0);
void                StartTest();
void                Plus(int* a, int* b, int* answer);
void                Minus(int* a, int* b, int* answer);
void                Multiply(int* a, int* b, int* answer);
void                Divide(int* a, int* b, int* answer);
void                SetBackground(HWND m_hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // TODO: 在此处放置代码。
    srand((unsigned)time(NULL));
    answerState = State::TIMER_OFF;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.hwnd == editText)
        {
            switch (msg.message)
            {
            case WM_KEYDOWN:
            {
                if (VK_RETURN == msg.wParam)
                {
                    wchar_t iat[30];
                    int length = GetWindowTextLength(editText);
                    GetWindowText(editText, iat, length + 1);
                    char* input = LPWSTRToCharArray(iat);
                    char* answer = new char[length];
                    string msg;
                    int bits = GetBits(*result);
                    _itoa_s(*result, answer, bits + 1, 10);
                    answerState = State::INCORRECT;
                    for (int i = 0; i < length; i++)
                    {
                        if (input[i] != answer[i])
                        {
                            msg = "答案错误！正确答案应为" + to_string(*result);
                            wrong++;
                            answerState = State::INCORRECT;
                            SetWindowText(label2, StringToLPCWSTR(msg));
                            msg = "得分：\n正确" + to_string(correct) + "个\n错误" + to_string(wrong) + "个";
                            SetWindowText(label3, StringToLPCWSTR(msg));
                            ShowWindow(label2, SW_HIDE);
                            ShowWindow(label2, SW_SHOW);
                            ShowWindow(label3, SW_HIDE);
                            ShowWindow(label3, SW_SHOW);
                            SetTimer(MainForm, 1, 1000, Timer_Tick);
                            timerState = State::TIMER_ON;
                            break;
                        }
                        if (i == length - 1)
                        {
                            msg = "恭喜你，答对了！";
                            correct++;
                            answerState = State::CORRECT;
                            SetWindowText(label2, StringToLPCWSTR(msg));
                            msg = "得分：\n正确" + to_string(correct) + "个\n错误" + to_string(wrong) + "个";
                            SetWindowText(label3, StringToLPCWSTR(msg));
                            ShowWindow(label2, SW_HIDE);
                            ShowWindow(label2, SW_SHOW);
                            ShowWindow(label3, SW_HIDE);
                            ShowWindow(label3, SW_SHOW);
                            SetTimer(MainForm, 1, 500, Timer_Tick);
                            timerState = State::TIMER_ON;
                        }
                    }
                }
            }
            break;
            case WM_LBUTTONDOWN:
            {
                wchar_t* cache = new wchar_t[GetWindowTextLength(editText) + 1];
                GetWindowText(editText, cache, GetWindowTextLength(editText) + 1);
                string text = LPWSTRToCharArray(cache);
                if (text == "在此输入答案")
                {
                    SetWindowText(editText, L"");
                }
            }
            break;
            default:
                break;
            }
        }
        if (true)
        {

        }
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;
   userControls.reserve(10);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, startPositionX, startPositionY, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MainForm = hWnd;
    switch (message)
    {
    case WM_CREATE: 
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        RECT rect;
        rect.right = windowWidth;
        rect.bottom = windowHeight;
        rect.left = (screenWidth - rect.right) / 2;
        rect.top = (screenHeight - rect.bottom) / 2;
        SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
        //backGroundImage = CreateWindow(L"static", TEXT("请点击任一模式开始练习"), WS_CHILD | WS_VISIBLE | SS_CENTER, 120, 10, 450, 50, hWnd, (HMENU)LABEL1, hInst, NULL);
        label1 = CreateWindow(L"static", TEXT("请点击任一模式开始练习"), WS_CHILD | WS_VISIBLE | SS_CENTER, 120, 10, 450, 50, hWnd, (HMENU)LABEL1, hInst, NULL);
        label2 = CreateWindow(L"static", TEXT("评价"), WS_CHILD | SS_CENTER, 120, 10, 450, 50, hWnd, (HMENU)LABEL2, hInst, NULL);
        label3 = CreateWindow(L"static", TEXT("得分：\n正确0个\n错误0个"), WS_CHILD | SS_CENTER, 120, 10, 450, 300, hWnd, (HMENU)LABEL3, hInst, NULL);
        labelFont = CreateFont(45, 0, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
        pointFont = CreateFont(28, 0, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
        brush = CreateSolidBrush(RGB(0x41, 0x96, 0x4F)); //翠绿色
        SendMessage(label1, WM_SETFONT, (WPARAM)labelFont, TRUE);
        SendMessage(label2, WM_SETFONT, (WPARAM)labelFont, TRUE);
        SendMessage(label3, WM_SETFONT, (WPARAM)pointFont, TRUE);
        int btn1_x = 300;
        int btn1_y = 120;
        int btn_gap = 10;
        int btn_width = 200;
        int btn_height = 60;
        button1 = CreateWindow(L"Button", L"加法模式：A＋B", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, btn1_x, btn1_y, btn_width, btn_height, hWnd, (HMENU)BUTTON1, hInst, NULL);
        button2 = CreateWindow(L"Button", L"减法模式：A－B", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, btn1_x, btn1_y + btn_height + btn_gap, btn_width, btn_height, hWnd, (HMENU)BUTTON2, hInst, NULL);
        button3 = CreateWindow(L"Button", L"乘法模式：A×B", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, btn1_x, btn1_y + (btn_height + btn_gap) * 2, btn_width, btn_height, hWnd, (HMENU)BUTTON3, hInst, NULL);
        button4 = CreateWindow(L"Button", L"除法模式：A÷B", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, btn1_x, btn1_y + (btn_height + btn_gap) * 3, btn_width, btn_height, hWnd, (HMENU)BUTTON4, hInst, NULL);
        button5 = CreateWindow(L"Button", L"确定", WS_CHILD | BS_PUSHBUTTON, btn1_x, btn1_y + (btn_height + btn_gap) * 4, 450, btn_height, hWnd, (HMENU)BUTTON5, hInst, NULL);
        button_exit = CreateWindow(L"Button", L"退出程序", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, windowWidth - 90, windowHeight - 116, 80, 60, hWnd, (HMENU)BUTTON_EXIT, hInst, NULL);
        btnFont = CreateFont(25, 0, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
        SendMessage(button1, WM_SETFONT, (WPARAM)btnFont, TRUE);
        SendMessage(button2, WM_SETFONT, (WPARAM)btnFont, TRUE);
        SendMessage(button3, WM_SETFONT, (WPARAM)btnFont, TRUE);
        SendMessage(button4, WM_SETFONT, (WPARAM)btnFont, TRUE);
        SendMessage(button5, WM_SETFONT, (WPARAM)btnFont, TRUE);
        SendMessage(button_exit, WM_SETFONT, (WPARAM)btnFont, TRUE);
        editText = CreateWindow(L"edit", L"在此输入答案", WS_CHILD | WS_BORDER, 0, 0, 450, 50, hWnd, (HMENU)EDITTEXT, hInst, NULL);
        userControls.push_back(label1);
        userControls.push_back(label2);
        userControls.push_back(label3);
        userControls.push_back(button1);
        userControls.push_back(button2);
        userControls.push_back(button3);
        userControls.push_back(button4);
        userControls.push_back(button5);
        userControls.push_back(button_exit);
        userControls.push_back(editText);
        CenterDisplay(userControls);
        GetClientRect(hWnd, &rect);
        windowWidth = rect.right - rect.left;
        windowHeight = rect.bottom - rect.top;
        MoveWindow(button_exit, windowWidth - 80, windowHeight - 60, 80, 60, TRUE);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_MainMenu:
        {
            if (timerState == State::TIMER_ON)
            {
                KillTimer(hWnd, 1);
            }
            for (HWND control:userControls)
            {
                ShowWindow(control, SW_SHOW);
            }
            SetWindowText(label1, L"请点击任一模式开始练习");
            ShowWindow(editText, SW_HIDE);
            ShowWindow(button5, SW_HIDE);
            ShowWindow(label1, SW_HIDE);
            ShowWindow(label1, SW_SHOW);
            ShowWindow(label2, SW_HIDE);
            ShowWindow(label3, SW_HIDE);
        }
        break;
        case BUTTON1:
        {
            /*MessageBox(hWnd, L"您点击了第一个按钮。", L"提示", MB_OK | MB_ICONINFORMATION);
            SendMessage((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, (LPARAM)L"第一个按钮已点击");*/
            mode = 1;
            StartTest();
        }
        break;
        case BUTTON2:
        {
            mode = 2;
            StartTest();
        }
        break;
        case BUTTON3:
        {
            mode = 3;
            StartTest();
        }
        break;
        case BUTTON4: 
        {
            mode = 4;
            StartTest();
        }
        break;
        case BUTTON5:
        {
            wchar_t iat[30];
            int length = GetWindowTextLength(editText);
            GetWindowText(editText, iat, length + 1);
            char* input = LPWSTRToCharArray(iat);
            char* answer = new char[length];
            string msg;
            int bits = GetBits(*result);
            _itoa_s(*result, answer, bits+1, 10);
            answerState = State::INCORRECT;
            for (int i = 0; i < length; i++)
            {
                if (input[i] != answer[i])
                {
                    msg = "答案错误！正确答案应为" + to_string(*result);
                    wrong++;
                    answerState = State::INCORRECT;
                    SetWindowText(label2, StringToLPCWSTR(msg));
                    msg = "得分：\n正确" + to_string(correct) + "个\n错误" + to_string(wrong) + "个";
                    SetWindowText(label3, StringToLPCWSTR(msg));
                    ShowWindow(label2, SW_HIDE);
                    ShowWindow(label2, SW_SHOW);
                    ShowWindow(label3, SW_HIDE);
                    ShowWindow(label3, SW_SHOW);
                    SetTimer(MainForm, 1, 1000, Timer_Tick);
                    timerState = State::TIMER_ON;
                    break;
                }
                if (i == length - 1)
                {
                    msg = "恭喜你，答对了！";
                    correct++;
                    answerState = State::CORRECT;
                    SetWindowText(label2, StringToLPCWSTR(msg));
                    msg = "得分：\n正确" + to_string(correct) + "个\n错误" + to_string(wrong) + "个";
                    SetWindowText(label3, StringToLPCWSTR(msg));
                    ShowWindow(label2, SW_HIDE);
                    ShowWindow(label2, SW_SHOW);
                    ShowWindow(label3, SW_HIDE);
                    ShowWindow(label3, SW_SHOW);
                    SetTimer(MainForm, 1, 500, Timer_Tick);
                    timerState = State::TIMER_ON;
                }
            }
            SetTimer(hWnd, 1, 1000, Timer_Tick);
            timerState = State::TIMER_ON;
        }
        break;
        case BUTTON_EXIT:
        {
            DestroyWindow(hWnd);
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CTLCOLORSTATIC: 
    {
        if ((HWND)lParam == GetDlgItem(hWnd, LABEL1))//获得指定标签句柄用来对比
        {
            SetTextColor((HDC)wParam, RGB(0, 0, 0));//设置文本颜色
            SetBkMode((HDC)wParam, TRANSPARENT);//设置背景透明
            //return (INT_PTR)brush;
        }
        if ((HWND)lParam == GetDlgItem(hWnd, LABEL2))
        {
            if (answerState == State::CORRECT)
            {
                SetTextColor((HDC)wParam, RGB(0, 255, 0));
            }
            else
            {
                SetTextColor((HDC)wParam, RGB(255, 0, 0));
            }
        }
        return (INT_PTR)GetStockObject((NULL_BRUSH));//返回一个空画刷(必须)
    }
    break;
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        windowWidth = rect.right - rect.left;
        windowHeight = rect.bottom - rect.top;
        CenterDisplay(userControls);
        MoveWindow(button_exit, windowWidth - 80, windowHeight - 60, 80, 60, TRUE);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        SetBackground(hWnd);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        DeleteObject(labelFont);
        DeleteObject(btnFont);
        DeleteObject(pointFont);
        if (timerState == State::TIMER_ON)
        {
            KillTimer(MainForm, 1);
            timerState = State::TIMER_OFF;
        }
        PostQuitMessage(0);
    }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON1:
        case IDCANCEL:
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
        default:
            break;
        }
    }
    break;
    }
    return (INT_PTR)FALSE;
}

inline char* LPWSTRToCharArray(LPWSTR lpString)
{
    LPSTR pszOut = NULL;
    if (lpString != NULL)
    {
        int nInputStrLen = wcslen(lpString);
        int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpString, nInputStrLen, NULL, 0, 0, 0) + 2;
        pszOut = new char[nOutputStrLen];

        if (pszOut)
        {
            memset(pszOut, 0x00, nOutputStrLen);
            WideCharToMultiByte(CP_ACP, 0, lpString, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
        }
    }
    return pszOut;
}

inline void CenterDisplay(vector<HWND> controls) {
    for (int i = 0; i < (int)controls.size(); i++)
    {
        MoveToCenter(controls.at(i), 1, 0);
    }
}

void StartTest() {
    int* a = new int;
    int* b = new int;
    *a = 1;
    *b = 2;
    ShowWindow(button1, SW_HIDE);
    ShowWindow(button2, SW_HIDE);
    ShowWindow(button3, SW_HIDE);
    ShowWindow(button4, SW_HIDE);
    ShowWindow(editText, SW_SHOW); 
    ShowWindow(button5, SW_SHOW);
    ShowWindow(label1, SW_HIDE);
    ShowWindow(label1, SW_SHOW);
    ShowWindow(label3, SW_SHOW);
    switch (mode)
    {
    case 1:
    {
        Plus(a, b, result);
    }
    break;
    case 2:
    {
        Minus(a, b, result);
    }
    break;
    case 3:
    {
        Multiply(a, b, result);
    }
    break;
    case 4:
    {
        Divide(a, b, result);
    }
    break;
    default:
        break;
    }
    char c1[15];
    char c2[15];
    _itoa_s(*a, c1, 10);
    string cache;
    if (*a < 0)
    {
        cache = "(";
        cache += c1;
        cache += ")";
    }
    else
    {
        cache = c1;
    }
    switch (mode)
    {
    case 1:
        cache += "+";
        break;
    case 2:
        cache += "-";
    case 3:
        cache += "*";
        break;
    case 4:
        cache += "/";
        break;
        break;
    default:
        break;
    }
    _itoa_s(*b, c2, 10);
    if (*b < 0)
    {
        cache += "(";
        cache += c2;
        cache += ")";
    }
    else
    {
        cache += c2;
    }
    cache += "=";
    MoveToCenter(label1, 1, 1, 0, -200);
    MoveToCenter(label2, 1, 1, 0, -50);
    MoveToCenter(label3, 1, 1, 0, 200);
    MoveToCenter(editText, 1, 1, 0, -100);
    MoveToCenter(button5, 1, 1, 0, 0);
    SetWindowText(label1, StringToLPCWSTR(cache));
}

inline void MoveToCenter(HWND hwnd, int horizontal, int vertical, int hoffset, int voffset) {
    RECT rect;
    GetWindowRect(hwnd, &rect);
    MapWindowPoints(NULL, MainForm, (LPPOINT)&rect, 2);
    int x = rect.left;
    int y = rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    if (horizontal > 0)
    {
        x = (windowWidth - width) / 2 + hoffset;
    }
    else
    {
        x += hoffset;
    }
    if (vertical > 0)
    {
        y = (windowHeight - height) / 2 + voffset;
    }
    else
    {
        y += voffset;
    }
    MoveWindow(hwnd, x, y, width, height, TRUE);
}

inline LPCWSTR StringToLPCWSTR(string orig)
{
    char* pCStrKey = const_cast<char*>(orig.c_str());
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}

void SetBackground(HWND m_hWnd/*, const TCHAR* pBackImgFullPath*/)
{
    //加载图片
    Gdiplus::Image* pImage = Gdiplus::Image::FromFile(L"D:\\d39ebca105927e9b04a96b31e83feebf.jpg");
    /*Image* pImage;
    HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(IDB_PNG1), L"PNG");
    if (hRsrc == NULL)
        return;
    DWORD dwLen = SizeofResource(hInst, hRsrc);
    BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
    if (!lpRsrc)
        return;;
    HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, dwLen);
    BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
    memcpy(pmem, lpRsrc, dwLen);
    IStream* pstm;
    CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
    pImage = Image::FromStream(pstm);
    GlobalUnlock(m_hMem);
    pstm->Release();
    FreeResource(lpRsrc);*/
    if (pImage == NULL)
    {
        assert(false && _T("背景图片打开失败!"));
    }

    RECT windowRect;
    GetWindowRect(m_hWnd, &windowRect);
    SIZE sizeWindow;
    if (windowRect.left == windowRect.right)
    {
        sizeWindow.cx = pImage->GetWidth();
        sizeWindow.cy = pImage->GetHeight();
    }
    else
    {
        sizeWindow.cx = windowRect.right - windowRect.left;
        sizeWindow.cy = windowRect.bottom - windowRect.top;
    }

    HDC hDC = ::GetDC(m_hWnd);
    HDC hdcMemory = CreateCompatibleDC(hDC);
    RECT rcWindow;
    GetWindowRect(m_hWnd, &rcWindow);

    BITMAPINFOHEADER stBmpInfoHeader = { 0 };
    int nBytesPerLine = ((sizeWindow.cx * 32 + 31) & (~31)) >> 3;
    stBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    stBmpInfoHeader.biWidth = sizeWindow.cx;
    stBmpInfoHeader.biHeight = sizeWindow.cy;
    stBmpInfoHeader.biPlanes = 1;
    stBmpInfoHeader.biBitCount = 32;
    stBmpInfoHeader.biCompression = BI_RGB;
    stBmpInfoHeader.biClrUsed = 0;
    stBmpInfoHeader.biSizeImage = nBytesPerLine * sizeWindow.cy;

    PVOID pvBits = NULL;
    HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);
    assert(hbmpMem != NULL);

    HGDIOBJ hbmpOld = ::SelectObject(hdcMemory, hbmpMem);
    POINT ptWinPos = { rcWindow.left, rcWindow.top };

    Gdiplus::Graphics graph(hdcMemory);
    graph.SetSmoothingMode(Gdiplus::SmoothingModeNone);
    graph.DrawImage(pImage, 0, 0, sizeWindow.cx, sizeWindow.cy);
    graph.FillRectangle(&SolidBrush(Color::Gray), 0, 0, 100, 50); //用GDI+在画布上画图

    HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));
    typedef BOOL(WINAPI* MYFUNC)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
    MYFUNC UpdateLayeredWindow;
    UpdateLayeredWindow = (MYFUNC)::GetProcAddress(hFuncInst, "UpdateLayeredWindow");
    POINT ptSrc = { 0, 0 };
    BLENDFUNCTION blendFunc;
    blendFunc.BlendOp = 0;
    blendFunc.BlendFlags = 0;
    blendFunc.AlphaFormat = 1;
    blendFunc.SourceConstantAlpha = 255;//AC_SRC_ALPHA
    //不会发送 WM_SIZE和WM_MOVE消息
    if (!UpdateLayeredWindow(m_hWnd, hDC, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &blendFunc, ULW_ALPHA))
    {
        assert(L"UpdateLayeredWindow 调用失败");
        TCHAR tmp[255] = { _T('\0') };
    }


    delete pImage;
    graph.ReleaseHDC(hdcMemory);
    ::SelectObject(hdcMemory, hbmpOld);

    ::DeleteObject(hFuncInst);
    ::DeleteObject(hbmpOld);
    ::DeleteObject(hbmpMem);
    ::DeleteDC(hdcMemory);
    ::DeleteDC(hDC);

}

void  CALLBACK Timer_Tick(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)//回调函数
{
    SetWindowText(button5, StringToLPCWSTR("确定"));
    SetWindowText(label2, L"");
    SetWindowText(editText, L"");
    ShowWindow(label2, SW_HIDE);
    ShowWindow(label2, SW_SHOW);
    StartTest();
    if (timerState == State::TIMER_ON)
    {
        KillTimer(hwnd, 1);
        timerState = State::TIMER_OFF;
    }
}

int GetBits(int number)
{
    int count = 0;
    while (number / 10 != 0)
    {
        number = number / 10;
        count++;
    }
    count = count + 1;
    if (number < 0)
    {
        count++;
    }
    return count;
}

inline int random(int min, int max) {
    return (rand() % (max - min)) + min;
}

//本函数由XXX制作，学号为2019621XXX
void Plus(int* a, int* b, int* answer) {
    *a = random(-100, 100);
    *b = random(-100, 100);
    *answer = *a + *b;
    //在这里写下你的代码，并删除函数内的注释
    //假设现有如下算式
    //a＋b=answer
    //a、b分别表示两个加数
    //answer表示运算结果
    //注意：a、b为指针形式
    //本函数要求：
    //1.通过调用随机数方法，实现随机加法运算
    //2.将a、b分别指向两加数所对应的内存地址
    //3.将answer指向正确答案所对应的内存地址
}

//本函数由XXX制作，学号为2019621XXX
void Minus(int* a, int* b, int* answer) {
    *a = random(-100, 100);
    *b = random(-100, 100);
    *answer = *a - *b;
    //在这里写下你的代码，并删除函数内的注释
    //假设现有如下算式
    //a－b=answer
    //a、b分别表示两个加数
    //answer表示运算结果
    //注意：a、b为指针形式
    //本函数要求：
    //1.通过调用随机数方法，实现随机减法运算
    //2.将a、b分别指向被减数与减数所对应的内存地址
    //3.将answer指向正确答案所对应的内存地址
    //4.a、b、answer所对应的值必须为整数
}

//本函数由XXX制作，学号为2019621XXX
void Multiply(int* a, int* b, int* answer) {
    *a = random(-100, 100);
    *b = random(-100, 100);
    *answer = *a * *b;
    //在这里写下你的代码，并删除函数内的注释
    //假设现有如下算式
    //a×b=answer
    //a、b分别表示两个加数
    //answer表示运算结果
    //注意：a、b为指针形式
    //本函数要求：
    //1.通过调用随机数方法，实现随机乘法运算
    //2.将a、b分别指向两乘数所对应的内存地址
    //3.将answer指向正确答案所对应的内存地址
    //4.a、b、answer所对应的值必须为整数
}

//本函数由XXX制作，学号为2019621XXX
void Divide(int* a, int* b, int* answer) {
    Multiply(answer, b, a);
    //在这里写下你的代码，并删除函数内的注释
    //假设现有如下算式
    //a÷b=answer
    //a、b分别表示两个加数
    //answer表示运算结果
    //注意：a、b为指针形式
    //本函数要求：
    //1.通过调用随机数方法，实现随机除法运算
    //2.将a、b分别指向被除数与除数所对应的内存地址
    //3.将answer指向正确答案所对应的内存地址
    //4.a、b、answer所对应的值必须为整数
}