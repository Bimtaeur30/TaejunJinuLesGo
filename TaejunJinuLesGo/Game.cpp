#include "Game.h"
#include "Arrow.h"
#include "Bow.h"
#include "Target.h"

void ClearCell(int x, int y)
{
    if (x < 1 || x >= CONSOLE_WIDTH - 1)  return;
    if (y < 1 || y >= CONSOLE_HEIGHT - 1) return;
    GotoXY(x, y);
    SetColor();
    cout << ' ';
}

void DrawBorder()
{
    SetColor(Color::LIGHT_GRAY);

    GotoXY(0, 0);
    cout << '+';
    for (int x = 1; x < CONSOLE_WIDTH - 1; ++x) cout << '-';
    cout << '+';

    GotoXY(0, CONSOLE_HEIGHT - 1);
    cout << '+';
    for (int x = 1; x < CONSOLE_WIDTH - 1; ++x) cout << '-';
    cout << '+';

    for (int y = 1; y < CONSOLE_HEIGHT - 1; ++y)
    {
        GotoXY(0, y);                 cout << '|';
        GotoXY(CONSOLE_WIDTH - 1, y); cout << '|';
    }

    SetColor();
}

void Init()
{
    SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleGameTitle(L"Arrow Shooting Game");
    SetConsoleMouseInputDisabled();

    system("cls");

    DrawBorder();
    DrawAmmoHUD();
    DrawBow(BOW_X, bowY);
    DrawTarget(TARGET_X, (int)targetY);
}