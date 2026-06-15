#include "Target.h"

float targetY = (float)(CONSOLE_HEIGHT / 2);
int   targetDir = 1;
int   targetBlinkFrames = 0;

void DrawTarget(int x, int y, bool erase)
{
    //  |O|
    //  |@|
    //  |O|
    bool blink = (targetBlinkFrames > 0) &&
        ((targetBlinkFrames / BLINK_INTERVAL_FRAMES) % 2 == 1);

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
}

void StartTargetBlink()
{
    targetBlinkFrames = BLINK_TOTAL_FRAMES;
}

void UpdateTarget()
{
    int oldY = (int)targetY;

    // 블링크 카운터 감소 및 색상 갱신
    if (targetBlinkFrames > 0)
    {
        --targetBlinkFrames;
        DrawTarget(TARGET_X, oldY); // 색상만 갱신 (위치는 그대로)
    }

    targetY += TARGET_SPEED * targetDir;

    if (targetY <= TARGET_MIN_Y) { targetY = (float)TARGET_MIN_Y; targetDir = 1; }
    if (targetY >= TARGET_MAX_Y) { targetY = (float)TARGET_MAX_Y; targetDir = -1; }

    int newY = (int)targetY;

    if (newY != oldY)
    {
        DrawTarget(TARGET_X, oldY, true);
        DrawTarget(TARGET_X, newY);
    }
}