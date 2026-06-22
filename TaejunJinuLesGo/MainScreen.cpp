#include "MainScreen.h"
#include "Console.h"

#include <Windows.h>
#include <shellapi.h>

#include <cmath>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Shell32.lib")

constexpr int MENU_W = 80;
constexpr int MENU_H = 35;

const std::vector<std::string> TITLE = {
    "   ___                        ___  ___          _            ",
    "  / _ \\                      |  \\/  |         | |           ",
    " / /_\\ \\_ __ _ __ _____      | .  . | __ _ ___| |_ ___ _ __ ",
    " |  _  | '__| '__/ _ \\ \\ /\\ / / |\\/| |/ _` / __| __/ _ \\ '__|",
    " | | | | |  | | | (_) \\ V  V /| |  | | (_| \\__ \\ ||  __/ |   ",
    " \\_| |_/_|  |_|  \\___/ \\_/\\_/ \\_|  |_/\\__,_|___/\\__\\___|_|  ",
};

void ClearMenuTarget(int cx, int cy)
{
    GotoXY(cx - 3, cy - 1);
    std::cout << "       ";

    GotoXY(cx - 3, cy);
    std::cout << "       ";

    GotoXY(cx - 3, cy + 1);
    std::cout << "       ";
}

void DrawMenuTarget(int cx, int cy)
{
    SetColor(Color::LIGHT_RED);

    GotoXY(cx - 3, cy - 1);
    std::cout << "( === )";

    GotoXY(cx - 3, cy);
    std::cout << "( [O] )";

    GotoXY(cx - 3, cy + 1);
    std::cout << "( === )";

    SetColor();
}

void DrawArrowDecoration()
{
    SetColor(Color::LIGHT_YELLOW);

    GotoXY(2, 18);
    std::cout << "--->";

    GotoXY(74, 18);
    std::cout << "<---";

    GotoXY(2, 20);
    std::cout << "--->";

    GotoXY(74, 20);
    std::cout << "<---";

    SetColor();
}

void DrawMenu()
{
    int startY = 25;
    int cx = MENU_W / 2;

    SetColor(Color::WHITE);
    GotoXY(cx - 20, startY);
    std::cout << "====================================";

    SetColor(Color::CYAN);
    GotoXY(cx - 10, startY + 2);
    std::cout << "[ 1 ] 게임 시작";

    GotoXY(cx - 10, startY + 3);
    std::cout << "[ 2 ] 소리 설정";

    GotoXY(cx - 10, startY + 4);
    std::cout << "[ 3 ] 게임 종료";

    SetColor(Color::WHITE);
    GotoXY(cx - 20, startY + 6);
    std::cout << "====================================";

    SetColor();
}

void DrawTitle()
{
    int startY = 3;
    int cx = MENU_W / 2;

    SetColor(Color::LIGHT_YELLOW);

    for (int i = 0; i < (int)TITLE.size(); ++i)
    {
        int x = cx - (int)TITLE[i].size() / 2;

        GotoXY(x, startY + i);
        std::cout << TITLE[i];
    }

    SetColor();
}

bool OpenWindowsSoundSettings()
{
    // Windows 10/11 설정 앱의 [시스템 > 소리] 화면을 연다.
    HINSTANCE result = ShellExecuteW(
        nullptr,
        L"open",
        L"ms-settings:sound",
        nullptr,
        nullptr,
        SW_SHOWNORMAL
    );

    // 설정 앱 실행이 막힌 환경이면 구형 제어판 소리 창으로 한 번 더 시도한다.
    if ((INT_PTR)result <= 32)
    {
        result = ShellExecuteW(
            nullptr,
            L"open",
            L"control.exe",
            L"mmsys.cpl",
            nullptr,
            SW_SHOWNORMAL
        );
    }

    return (INT_PTR)result > 32;
}

int ShowMainScreen()
{
    SetConsoleWindowSize(MENU_W, MENU_H);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleMouseInputDisabled();

    float targetX[2] = { 10.0f, 62.0f };
    float targetBaseX[2] = { 10.0f, 62.0f };
    int targetY[2] = { 16, 16 };
    float phase[2] = { 0.0f, 3.14159f };

    constexpr int MENU_FPS = 30;

    float t = 0.0f;

    system("cls");

    DrawTitle();
    DrawArrowDecoration();
    DrawMenu();

    for (int i = 0; i < 2; ++i)
    {
        DrawMenuTarget((int)targetX[i], targetY[i]);
    }

    while (true)
    {
        UpdateInput();

        for (int i = 0; i < 2; ++i)
        {
            int prevX = (int)targetX[i];

            targetX[i] = targetBaseX[i] + sinf(t + phase[i]) * 6.0f;

            int nextX = (int)targetX[i];

            if (prevX != nextX)
            {
                ClearMenuTarget(prevX, targetY[i]);
                DrawMenuTarget(nextX, targetY[i]);
            }
        }

        if (_kbhit())
        {
            char ch = (char)_getch();

            if (ch == '1') return 1;
            if (ch == '2') return 2;
            if (ch == '3') return 3;
        }

        t += 0.08f;

        FrameSync(MENU_FPS);
    }
}