#include "Game.h"
#include "Arrow.h"
#include "Bow.h"
#include "Target.h"

#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <cstdlib>

void ClearCell(int x, int y)
{
    if (x < 1 || x >= CONSOLE_WIDTH - 1) return;
    if (y < 1 || y >= CONSOLE_HEIGHT - 1) return;

    GotoXY(x, y);
    SetColor();
    std::cout << ' ';
}

void DrawBorder()
{
    SetColor(Color::LIGHT_GRAY);

    GotoXY(0, 0);
    std::cout << '+';
    for (int x = 1; x < CONSOLE_WIDTH - 1; ++x)
    {
        std::cout << '-';
    }
    std::cout << '+';

    GotoXY(0, CONSOLE_HEIGHT - 1);
    std::cout << '+';
    for (int x = 1; x < CONSOLE_WIDTH - 1; ++x)
    {
        std::cout << '-';
    }
    std::cout << '+';

    for (int y = 1; y < CONSOLE_HEIGHT - 1; ++y)
    {
        GotoXY(0, y);
        std::cout << '|';

        GotoXY(CONSOLE_WIDTH - 1, y);
        std::cout << '|';
    }

    SetColor();
}

void ResetGame()
{
    ammo = MAX_AMMO;
    bowY = CONSOLE_HEIGHT / 2;

    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        arrows[i] = {};
    }
}

void StartStage(int stage)
{
    system("cls");

    DrawBorder();

    InitStage(stage);

    ammo = MAX_AMMO;

    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        arrows[i].active = false;
    }

    DrawAmmoHUD();
    DrawStageHUD();
    DrawBow(BOW_X, bowY);

    for (int i = 0; i < targetCount; ++i)
    {
        DrawTarget(i);
    }
}

void Init()
{
    SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleGameTitle(L"Arrow Shooting Game");
    SetConsoleMouseInputDisabled();

    ResetGame();
    StartStage(1);
}

void RunGame()
{
    Init();

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE))
        {
            system("cls");
            return;
        }

        HandleInput();
        UpdateArrows();
        UpdateTargets();

        if (AllTargetsCleared())
        {
            ShowStageClear(currentStage);

            int nextStage = currentStage + 1;

            if (nextStage > MAX_STAGE)
            {
                ShowGameClear();
                system("cls");
                return;
            }

            StartStage(nextStage);
        }

        DrawStageHUD();

        FrameSync(FPS);
    }
}

void ShowStageClear(int stage)
{
    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 14, cy - 1);
    std::cout << "============================";

    GotoXY(cx - 14, cy);
    std::cout << "      STAGE " << stage << " CLEAR!      ";

    GotoXY(cx - 14, cy + 1);
    std::cout << "============================";

    SetColor();

    Sleep(1200);
}

void ShowGameClear()
{
    system("cls");
    DrawBorder();

    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(cx - 14, cy - 2);
    std::cout << "*** CONGRATULATIONS! ***";

    SetColor(Color::WHITE);
    GotoXY(cx - 14, cy);
    std::cout << "All " << MAX_STAGE << " stages cleared!";

    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 14, cy + 2);
    std::cout << "Press any key to return menu...";

    SetColor();

    while (!_kbhit())
    {
        Sleep(100);
    }

    _getch();

    system("cls");
}