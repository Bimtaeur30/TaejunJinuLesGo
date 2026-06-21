#include "Game.h"
#include "Arrow.h"
#include "Bow.h"
#include "Target.h"
#include "Obstacle.h"
#include "Skill.h"
#include "Card.h"

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

// ───────────────────────────────────────────────
//  스테이지 시작: 화면 클리어 후 재초기화
// ───────────────────────────────────────────────
void StartStage(int stage)
{
    system("cls");

    DrawBorder();
    InitStage(stage);
    InitObstacles(stage);

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

    // 모든 장애물 그리기
    for (int i = 0; i < obstacleCount; ++i)
        DrawObstacle(i);

    // 스킬 박스 UI (cls로 지워졌으므로 다시 그림)
    DrawSkillBox();
}

// ───────────────────────────────────────────────
//  최초 Init (스테이지 1로 시작)
// ───────────────────────────────────────────────
void Init()
{
    SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleGameTitle(L"Arrow Shooting Game");
    SetConsoleMouseInputDisabled();

    InitSkills();
    StartStage(1);
}

// ───────────────────────────────────────────────
//  스테이지 클리어 연출 (잠깐 메시지 표시 후 다음 스테이지)
// ───────────────────────────────────────────────
void ShowStageClear(int stage)
{
    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 10, cy - 1); cout << "============================";
    GotoXY(cx - 10, cy);     cout << "  STAGE " << stage << " CLEAR!  ";
    GotoXY(cx - 10, cy + 1); cout << "============================";
    SetColor();

    Sleep(1800);
}

// ───────────────────────────────────────────────
//  전 스테이지 클리어 연출
// ───────────────────────────────────────────────

void ShowGameClear()
{
    system("cls");
    DrawBorder();

    int cx = CONSOLE_WIDTH / 2;
    int cy = CONSOLE_HEIGHT / 2;

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(cx - 14, cy - 2); cout << "  *** CONGRATULATIONS! ***  ";
    SetColor(Color::WHITE);
    GotoXY(cx - 14, cy);     cout << "  All " << MAX_STAGE << " stages cleared!  ";
    SetColor(Color::LIGHT_GREEN);
    GotoXY(cx - 14, cy + 2); cout << "  Press any key to exit...  ";
    SetColor();

    while (!_kbhit()) Sleep(100);
    _getch();
}  