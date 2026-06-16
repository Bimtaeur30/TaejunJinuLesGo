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

void StartStage(int stage)
{
    system("cls");

    DrawBorder();
    InitStage(stage);

    // 탄약 보충
    ammo = MAX_AMMO;
    // 화살 전부 비활성화
    for (int i = 0; i < MAX_ARROWS; ++i)
        arrows[i].active = false;

    DrawAmmoHUD();
    DrawStageHUD();
    DrawBow(BOW_X, bowY);

    // 모든 과녁 그리기
    for (int i = 0; i < targetCount; ++i)
        DrawTarget(i);
}

void Init()
{
    SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleGameTitle(L"Arrow Shooting Game");
    SetConsoleMouseInputDisabled();

    StartStage(1);
}

void ShowStageClear(int stage)
{
    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 10, cy - 1); cout << "============================";
    GotoXY(cx - 10, cy);     cout << "  스테이지 " << stage << " 클리어!  ";
    GotoXY(cx - 10, cy + 1); cout << "============================";

    SetColor();

    Sleep(1800);
}


void ShowGameClear()
{
    system("cls");
    DrawBorder();

    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(cx - 14, cy - 2); cout << "  *** 축하합니다! ***  ";
    SetColor(Color::WHITE);
    GotoXY(cx - 14, cy);     cout << "  모든 " << MAX_STAGE << "개의 스테이지 클리어!  ";
    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 14, cy + 2); cout << " 아무 키나 눌러 나가세요 ";
    SetColor();

    while (!_kbhit()) Sleep(100);
    _getch();
}