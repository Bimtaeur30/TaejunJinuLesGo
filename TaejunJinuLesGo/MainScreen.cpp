#include "Console.h"
#include <string>
#include <vector>
#include <cmath>

constexpr int W = 80;
constexpr int H = 35;

const std::vector<std::string> TITLE = {
    "   ___                        ___  ___          _            ",
    "  / _ \\                      |  \\/  |         | |           ",
    " / /_\\ \\_ __ _ __ _____      | .  . | __ _ ___| |_ ___ _ __ ",
    " |  _  | '__| '__/ _ \\ \\ /\\ / / |\\/| |/ _` / __| __/ _ \\ '__|",
    " | | | | |  | | | (_) \\ V  V /| |  | | (_| \\__ \\ ||  __/ |   ",
    " \\_| |_/_|  |_|  \\___/ \\_/\\_/ \\_|  |_/\\__,_|___/\\__\\___|_|  ",
};

void ClearTarget(int cx, int cy)
{
    GotoXY(cx - 3, cy - 1); std::cout << "       ";
    GotoXY(cx - 3, cy); std::cout << "       ";
    GotoXY(cx - 3, cy + 1); std::cout << "       ";
}

void DrawTarget(int cx, int cy)
{
    SetColor(Color::LIGHT_RED);
    GotoXY(cx - 3, cy - 1); std::cout << "( === )";
    GotoXY(cx - 3, cy); std::cout << "( [O] )";
    GotoXY(cx - 3, cy + 1); std::cout << "( === )";
    SetColor();
}

void DrawArrowDecoration()
{
    SetColor(Color::LIGHT_YELLOW);
    GotoXY(2, 18); std::cout << "--->";
    GotoXY(74, 18); std::cout << "<---";
    GotoXY(2, 20); std::cout << "--->";
    GotoXY(74, 20); std::cout << "<---";
    SetColor();
}

void DrawMenu()
{
    int startY = 25;
    int cx = W / 2;

    SetColor(Color::WHITE);
    GotoXY(cx - 20, startY); std::cout << "====================================";

    SetColor(Color::CYAN);
    GotoXY(cx - 10, startY + 2); std::cout << "[ 1 ] 게임 시작";
    GotoXY(cx - 10, startY + 3); std::cout << "[ 2 ] 소리 설정";
    GotoXY(cx - 10, startY + 4); std::cout << "[ 3 ] 게임 종료";

    SetColor(Color::WHITE);
    GotoXY(cx - 20, startY + 6); std::cout << "====================================";

    SetColor();
}

void DrawTitle()
{
    int startY = 3;
    int cx = W / 2;

    SetColor(Color::LIGHT_YELLOW);
    for (int i = 0; i < (int)TITLE.size(); ++i)
    {
        int x = cx - (int)TITLE[i].size() / 2;
        GotoXY(x, startY + i);
        std::cout << TITLE[i];
    }
    SetColor();
}

int ShowMainScreen()
{
    SetConsoleWindowSize(W, H);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleMouseInputDisabled();

    float targetX[2] = { 10.f, 62.f };
    float targetBaseX[2] = { 10.f, 62.f };
    int   targetY[2] = { 16, 16 };
    float phase[2] = { 0.f, 3.14159f };

    constexpr int FPS = 30;
    float t = 0.f;

    system("cls");
    DrawTitle();
    DrawArrowDecoration();
    DrawMenu();

    while (true)
    {
        UpdateInput();

        for (int i = 0; i < 2; ++i)
        {
            int prevX = (int)targetX[i];
            targetX[i] = targetBaseX[i] + sinf(t + phase[i]) * 6.f;
            int nextX = (int)targetX[i];

            if (prevX != nextX)
            {
                ClearTarget(prevX, targetY[i]);
                DrawTarget(nextX, targetY[i]);
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
        FrameSync(FPS);
    }
}

int main()
{
    srand((unsigned)time(nullptr));

    SetCursorVisible(true);
    while (true)
    {
        int choice = ShowMainScreen();
        system("cls");

        switch (choice)
        {
        case 1:
            SetColor(Color::LIGHT_GREEN);
            GotoXY(30, 13); std::cout << ">>  게임을 시작합니다!";
            SetColor();
            Sleep(1500);
            break;
        case 2:
            SetColor(Color::LIGHT_BLUE);
            GotoXY(30, 13); std::cout << "~  소리 설정";
            SetColor();
            Sleep(1500);
            break;
        case 3:
            SetColor(Color::LIGHT_RED);
            GotoXY(31, 13); std::cout << "x  게임을 종료합니다.";
            SetColor();
            Sleep(1000);
            return 0;
        }
    }
}