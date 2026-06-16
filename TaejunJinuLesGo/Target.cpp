#include "Target.h"
#include <cstdlib>
#include <cmath>

Target targets[MAX_TARGETS] = {};
int    targetCount = 0;
int    currentStage = 1;

static void EraseTargetAt(int x, int y)
{
    for (int dy = -1; dy <= 1; ++dy)
    {
        ClearCell(x, y + dy);
        ClearCell(x + 1, y + dy);
        ClearCell(x + 2, y + dy);
    }
}

void InitStage(int stage)
{
    currentStage = stage;
    targetCount = stage;

    int areaWidth = TARGET_AREA_RIGHT - TARGET_AREA_LEFT;
    int slot = (targetCount > 1) ? areaWidth / (targetCount - 1) : 0;

    for (int i = 0; i < MAX_TARGETS; ++i)
        targets[i] = {};

    for (int i = 0; i < targetCount; ++i)
    {
        int xPos = (targetCount == 1)
            ? (TARGET_AREA_LEFT + areaWidth / 2)
            : (TARGET_AREA_LEFT + slot * i);

        float base = TARGET_BASE_SPEED + TARGET_SPEED_STEP * (stage - 1);
        float jitter = ((float)(rand() % 9) - 4) * 0.015f;
        float spd = base + jitter;
        if (spd < 0.05f) spd = 0.05f;

        int midY = (TARGET_MIN_Y + TARGET_MAX_Y) / 2;
        int range = (TARGET_MAX_Y - TARGET_MIN_Y) / 2;
        int startY = midY + (range * (2 * i - (targetCount - 1))) / (targetCount + 1);

        targets[i].y = (float)startY;
        targets[i].x = xPos;
        targets[i].dir = (i % 2 == 0) ? 1 : -1;
        targets[i].speed = spd;
        targets[i].blinkFrames = 0;
        targets[i].active = true;
        targets[i].removing = false;
        targets[i].prevDrawY = startY;   // 초기 그린 Y
    }
}

void DrawTarget(int idx, bool erase)
{
    if (idx < 0 || idx >= targetCount) return;

    Target& t = targets[idx];
    int x = t.x;
    int y = (int)t.y;

    if (erase)
    {
        EraseTargetAt(x, y);
        return;
    }

    bool blink = (t.blinkFrames > 0) &&
        ((t.blinkFrames / BLINK_INTERVAL_FRAMES) % 2 == 1);

    for (int dy = -1; dy <= 1; ++dy)
    {
        GotoXY(x, y + dy);
        if (blink)
        {
            SetColor(Color::WHITE);
            cout << '|';
            SetColor(Color::WHITE);
            cout << (dy == 0 ? '@' : 'O');
            SetColor(Color::WHITE);
            cout << '|';
        }
        else
        {
            SetColor(Color::LIGHT_RED);
            cout << '|';
            SetColor(dy == 0 ? Color::LIGHT_YELLOW : Color::LIGHT_RED);
            cout << (dy == 0 ? '@' : 'O');
            SetColor(Color::LIGHT_RED);
            cout << '|';
        }
    }
    SetColor();

    // 그린 위치 기록
    t.prevDrawY = y;
}

void StartTargetBlink(int idx)
{
    if (idx < 0 || idx >= targetCount) return;
    targets[idx].blinkFrames = BLINK_TOTAL_FRAMES;
    targets[idx].removing = true;
}

void UpdateTargets()
{
    for (int i = 0; i < targetCount; ++i)
    {
        Target& t = targets[i];
        if (!t.active) continue;

        if (t.blinkFrames > 0) // 블링크
        {
            --t.blinkFrames;

            if (t.blinkFrames == 0 && t.removing)
            {
                EraseTargetAt(t.x, t.prevDrawY);
                t.active = false;
                continue;
            }

            DrawTarget(i);
            continue;   // 블링크 중에는 이동 ㄴㄴ
        }

        if (t.removing) continue;


        int drawnY = t.prevDrawY;

        t.y += t.speed * t.dir;
        if (t.y <= TARGET_MIN_Y) { t.y = (float)TARGET_MIN_Y; t.dir = 1; }
        if (t.y >= TARGET_MAX_Y) { t.y = (float)TARGET_MAX_Y; t.dir = -1; }

        int newY = (int)t.y;

        if (newY != drawnY)
        {
            EraseTargetAt(t.x, drawnY);
            DrawTarget(i);
        }
    }
}

bool AllTargetsCleared()
{
    for (int i = 0; i < targetCount; ++i)
        if (targets[i].active) return false;
    return true;
}

void DrawStageHUD()
{
    int remaining = 0;
    for (int i = 0; i < targetCount; ++i)
        if (targets[i].active) ++remaining;

    GotoXY(2, 1);
    SetColor(Color::LIGHT_YELLOW);
    cout << "Stage: ";
    SetColor(Color::WHITE);
    cout << currentStage << "/" << MAX_STAGE;

    SetColor(Color::LIGHT_YELLOW);
    cout << "   Targets: ";
    SetColor(Color::LIGHT_RED);
    cout << remaining << "/" << targetCount;
    cout << "   ";

    SetColor();
}