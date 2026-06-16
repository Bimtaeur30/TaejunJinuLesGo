#include "Target.h"
#include "Game.h"

#include <iostream>

Target targets[MAX_TARGETS] = {};
int targetCount = 0;
int currentStage = 1;

void InitStage(int stage)
{
    currentStage = stage;

    for (int i = 0; i < MAX_TARGETS; ++i)
    {
        targets[i] = {};
    }

    if (stage == 1)
    {
        targetCount = 1;

        targets[0].x = CONSOLE_WIDTH - 10;
        targets[0].y = CONSOLE_HEIGHT / 2.0f;
        targets[0].dir = 1;
        targets[0].speed = 0.20f;
        targets[0].active = true;
    }
    else if (stage == 2)
    {
        targetCount = 2;

        targets[0].x = CONSOLE_WIDTH - 12;
        targets[0].y = 8.0f;
        targets[0].dir = 1;
        targets[0].speed = 0.25f;
        targets[0].active = true;

        targets[1].x = CONSOLE_WIDTH - 22;
        targets[1].y = 20.0f;
        targets[1].dir = -1;
        targets[1].speed = 0.30f;
        targets[1].active = true;
    }
    else
    {
        targetCount = 3;

        targets[0].x = CONSOLE_WIDTH - 10;
        targets[0].y = 6.0f;
        targets[0].dir = 1;
        targets[0].speed = 0.30f;
        targets[0].active = true;

        targets[1].x = CONSOLE_WIDTH - 20;
        targets[1].y = 15.0f;
        targets[1].dir = -1;
        targets[1].speed = 0.35f;
        targets[1].active = true;

        targets[2].x = CONSOLE_WIDTH - 30;
        targets[2].y = 23.0f;
        targets[2].dir = 1;
        targets[2].speed = 0.28f;
        targets[2].active = true;
    }
}

void DrawStageHUD()
{
    SetColor(Color::WHITE);
    GotoXY(CONSOLE_WIDTH / 2 - 5, 1);
    std::cout << "STAGE " << currentStage << " / " << MAX_STAGE;
    SetColor();
}

void DrawTarget(int index, bool erase)
{
    if (index < 0 || index >= targetCount) return;

    Target& target = targets[index];

    if (!target.active && !erase) return;

    int x = target.x;
    int y = (int)target.y;

    bool blink =
        target.blinkFrames > 0 &&
        ((target.blinkFrames / BLINK_INTERVAL_FRAMES) % 2 == 1);

    for (int dy = -1; dy <= 1; ++dy)
    {
        if (erase)
        {
            ClearCell(x, y + dy);
            ClearCell(x + 1, y + dy);
            ClearCell(x + 2, y + dy);
            continue;
        }

        GotoXY(x, y + dy);

        if (blink)
        {
            SetColor(Color::WHITE);
            std::cout << '|';

            SetColor(Color::WHITE);
            std::cout << (dy == 0 ? '@' : 'O');

            SetColor(Color::WHITE);
            std::cout << '|';
        }
        else
        {
            SetColor(Color::LIGHT_RED);
            std::cout << '|';

            SetColor(dy == 0 ? Color::LIGHT_YELLOW : Color::LIGHT_RED);
            std::cout << (dy == 0 ? '@' : 'O');

            SetColor(Color::LIGHT_RED);
            std::cout << '|';
        }
    }

    SetColor();
}

void StartTargetBlink(int index)
{
    if (index < 0 || index >= targetCount) return;

    targets[index].blinkFrames = BLINK_TOTAL_FRAMES;
}

void DestroyTarget(int index)
{
    if (index < 0 || index >= targetCount) return;
    if (!targets[index].active) return;

    DrawTarget(index, true);
    targets[index].active = false;
}

void UpdateTarget()
{
    for (int i = 0; i < targetCount; ++i)
    {
        Target& target = targets[i];

        if (!target.active) continue;

        int oldY = (int)target.y;

        if (target.blinkFrames > 0)
        {
            --target.blinkFrames;
            DrawTarget(i);
        }

        target.y += target.speed * target.dir;

        if (target.y <= TARGET_MIN_Y)
        {
            target.y = (float)TARGET_MIN_Y;
            target.dir = 1;
        }

        if (target.y >= TARGET_MAX_Y)
        {
            target.y = (float)TARGET_MAX_Y;
            target.dir = -1;
        }

        int newY = (int)target.y;

        if (newY != oldY)
        {
            DrawTarget(i, true);
            DrawTarget(i);
        }
    }
}

int FindHitTarget(int arrowX, int arrowY)
{
    int headX = arrowX + 1;

    for (int i = 0; i < targetCount; ++i)
    {
        Target& target = targets[i];

        if (!target.active) continue;

        int tx = target.x;
        int ty = (int)target.y;

        bool hitX = headX >= tx && headX <= tx + 2;
        bool hitY = arrowY >= ty - 1 && arrowY <= ty + 1;

        if (hitX && hitY)
        {
            return i;
        }
    }

    return -1;
}

bool AreAllTargetsDead()
{
    for (int i = 0; i < targetCount; ++i)
    {
        if (targets[i].active)
        {
            return false;
        }
    }

    return true;
}
void UpdateTargets()
{
    UpdateTarget();
}

bool AllTargetsCleared()
{
    return AreAllTargetsDead();
}