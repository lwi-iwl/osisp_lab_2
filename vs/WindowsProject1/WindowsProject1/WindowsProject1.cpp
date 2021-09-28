#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <atlstr.h>
#include <list>
#include <sstream>
#include <fstream>
#include <codecvt>
#define PI 3.14159265


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND hNumOfRows;
HWND hNumOfColumns;
HWND hFontSize;
HDC winDC;
RECT rect;
PAINTSTRUCT ps;
bool isFirstPaint = true;
int rows = 0;
int columns = 0;
int sizefont = 0;
std::string line;
std::wstring circleline;
std::list<std::wstring> strings = {};
std::list<std::wstring> circlestrings = {};
LPTEXTMETRIC lptm;
RECT tablerect;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow)
{
    WNDCLASS windowClass = { 0 };
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = L"HELLO_WORLD";
    windowClass.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW + 1);
    RegisterClass(&windowClass);
    HWND hwnd = CreateWindow(
        windowClass.lpszClassName,
        L"CatXP",
        WS_OVERLAPPEDWINDOW ,
        100, 50, 1280, 720,
        nullptr, nullptr,
        hInstance,
        nullptr);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
    {
        hNumOfRows = CreateWindowEx(NULL, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_NUMBER,
            10, 20, 50, 20, hWnd, (HMENU)1, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hNumOfColumns = CreateWindowEx(NULL, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_NUMBER,
            70, 20, 50, 20, hWnd, (HMENU)2, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
        hFontSize = CreateWindowEx(NULL, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_NUMBER,
            130, 20, 50, 20, hWnd, (HMENU)3, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

        SendMessage(hNumOfRows, EM_SETLIMITTEXT, 2, 0);
        SendMessage(hNumOfColumns, EM_SETLIMITTEXT, 2, 0);
        SendMessage(hFontSize, EM_SETLIMITTEXT, 2, 0);
    }
        break;

    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        rect.top = 42;
        winDC = BeginPaint(hWnd, &ps);
        if (isFirstPaint)
        {
            std::ifstream in("C:\\Docs\\OS\\2\\vs\\WindowsProject1\\Debug\\1.txt");
            if (in.is_open())
            {
                while (getline(in, line))
                {
                    std::cout << line << std::endl;
                    std::wstring stemp = std::wstring(line.begin(), line.end());
                    strings.push_back(stemp);
                }
            }
            in.close();
            std::wifstream circlein("C:\\Docs\\OS\\2\\vs\\WindowsProject1\\Debug\\2.txt");
            if (circlein.is_open())
            {
                circlein.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
                std::wstringstream wss;
                wss << circlein.rdbuf();
                circleline = wss.str();
                //MessageBox(hWnd, circleline.c_str(), L"hjhj", NULL);
            }
            circlein.close();
            TextOut(winDC, 10, 0, L"Rows", 4);
            TextOut(winDC, 70, 0, L"Columns", 7);
            TextOut(winDC, 130, 0, L"Font Size", 9);
            MoveToEx(winDC, 0, 42, NULL);
            LineTo(winDC, 1280, 42);
            //MessageBox(hWnd, stemp.c_str(), L"hjhj", NULL);
            isFirstPaint = false;
        }
        FillRect(winDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        int oldheight = rect.top;
        if (rows != 0)
        {
            GetTextMetrics(winDC, lptm);
            int counter = 0;
            int rowscounter = 1;
            int columnscounter = 1;
            int height = 0;
            std::list<std::wstring>::iterator it;
            HFONT hFont = CreateFont(sizefont, 0, 0, 0, 400,
                0, 0, 0, 0, 0,
                0, 0, 0, 0);
            SelectObject(winDC, hFont);
            for (it = strings.begin(); it != strings.end(); ++it) {
                if (columnscounter <= columns * rows)
                {
                    if (counter >= columns)
                    {
                        oldheight = height + oldheight+5;
                        MoveToEx(winDC, 0, oldheight, NULL);
                        LineTo(winDC, rect.right, oldheight);
                        oldheight += 5;
                        counter = 0;
                        height = 0;
                        rowscounter++;
                    }

                    tablerect.top = oldheight;
                    tablerect.left = counter * rect.right / columns + 5;
                    tablerect.right = (counter + 1) * rect.right / columns - 5;
                    tablerect.bottom = 0;
                    int rowk = DrawText(winDC, it->c_str(), wcslen(it->c_str()), &tablerect, DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL);
                    tablerect.bottom = tablerect.top + rowk;
                    rowk = DrawText(winDC, it->c_str(), wcslen(it->c_str()), &tablerect, DT_WORDBREAK | DT_EDITCONTROL);
                    if (rowk > height)
                        height = rowk;
                    counter++;
                    columnscounter++;
                }
            }
            DeleteObject(hFont);
            oldheight = height + oldheight + 5;
            MoveToEx(winDC, 0, oldheight, NULL);
            LineTo(winDC, rect.right, oldheight);
            for (int i = rowscounter + 1; i <= rows; i++) 
            {
                oldheight = oldheight + 5;
                MoveToEx(winDC, 0, oldheight, NULL);
                LineTo(winDC, rect.right, oldheight);
            }

        }

        if (columns != 0)
        {
            for (int i = 1; i < columns; i++)
            {
                MoveToEx(winDC, rect.right *i / columns, rect.top, NULL);
                LineTo(winDC, rect.right *i / columns, oldheight);
            }
        }
        std::list<std::wstring>::iterator it;
        int delta = 80;
        int angle = 900;
        int x = 180;
        int y = oldheight + 320;
        int dkoeff = 180;
        int i = 0;
        while ((dkoeff > 50)&&(i < wcslen(circleline.c_str())))
        {
            HFONT hFont = CreateFont(20, 0, angle - 900, 0, 400,
                0, 0, 0, 0, 0,
                0, 0, 0, 0);
            SelectObject(winDC, hFont);
            WCHAR wch[2] = { circleline.c_str()[i] };
            LPWSTR lpwstr = wch;
                        
            TextOut(winDC, x + cos(angle * PI / 1800) * dkoeff, y - sin(angle * PI / 1800) * dkoeff, lpwstr, 1);
            DeleteObject(hFont);
            if (angle <= -2600)
            {
                dkoeff -= 20;
                Ellipse(winDC, x - dkoeff, y - dkoeff, x + dkoeff, y + dkoeff);
                dkoeff -= 20;
                angle = 900;
                delta += 30;
            }
            angle = angle - delta;
            i++;
        }

        MoveToEx(winDC, 0, 42, NULL);
        LineTo(winDC, rect.right, 42);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
        break;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case 1:
        {
            switch (HIWORD(wParam))
            {
                case EN_CHANGE:
                {
                    wchar_t wc[255];
                    LPWSTR buff;
                    GetWindowText((HWND)lParam, wc, 255);
                    std::wstring ws(wc);
                    std::string s = std::string(ws.begin(), ws.end());
                    if (s != "")
                        rows = std::stoi(s);
                    else
                        rows = 0;
                    //MessageBox(NULL, LPWSTR(Buffer), L"Error", MB_OK | MB_ICONERROR);
                    InvalidateRect(hWnd, &rect, NULL);
                }
            }
        }
        break;

        case 2:
        {
            switch (HIWORD(wParam))
            {
                case EN_CHANGE:
                {
                    wchar_t wc[255];
                    LPWSTR buff;
                    GetWindowText((HWND)lParam, wc, 255);
                    std::wstring ws(wc);
                    std::string s = std::string(ws.begin(), ws.end());
                    if (s != "")
                        columns = std::stoi(s);
                    else
                        columns = 0;
                    //MessageBox(NULL, LPWSTR(Buffer), L"Error", MB_OK | MB_ICONERROR);
                    InvalidateRect(hWnd, &rect, NULL);
                }
            }
        }
        break;
        case 3:
        {
            switch (HIWORD(wParam))
            {
            case EN_CHANGE:
            {
                wchar_t wc[255];
                LPWSTR buff;
                GetWindowText((HWND)lParam, wc, 255);
                std::wstring ws(wc);
                std::string s = std::string(ws.begin(), ws.end());
                if (s != "")
                    sizefont = std::stoi(s);
                else
                    sizefont = 0;
                //MessageBox(NULL, LPWSTR(Buffer), L"Error", MB_OK | MB_ICONERROR);
                InvalidateRect(hWnd, &rect, NULL);
            }
            }
        }
        }
        return 0;
    }
    break;

    case WM_SIZE: 
    {
        InvalidateRect(hWnd, &rect, NULL);
    }
    break;

    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMinTrackSize.x = 300;
        lpMMI->ptMinTrackSize.y = 300;
    }

    default: {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }

    return 0;
}
