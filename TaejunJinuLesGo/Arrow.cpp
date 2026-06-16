#include "Arrow.h"
#include "Target.h"

#include <cmath>
#include <iostream>
#include <Windows.h>

Arrow arrows[MAX_ARROWS] = {};
int ammo = MAX_AMMO;

void SpawnArrow(int startX, int startY)
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active)
        {
            float fx = (float)startX + 1.0f;
            float fy = (float)startY;

            arrows[i].x = fx;
            arrows[i].y = fy;
            arrows[i].prevX = fx;
            arrows[i].prevY = fy;
            arrows[i].active = true;

            return;
        }
    }
}

static void ArrowGlyph(float dx, float dy, char& c0, char& c1)
{
    float ax = dx;
    float ay = dy * 0.5f;
    float angle = atan2f(ay, ax);

    const float P = 3.14159f;
    const float PI8 = P / 8.0f;

    if (angle > -PI8 && angle <= PI8)
    {
        c0 = '-';
        c1 = '>';
    }
    else if (angle > PI8 && angle <= 3 * PI8)
    {
        c0 = '\\';
        c1 = '>';
    }
    else if (angle > 3 * PI8 && angle <= 5 * PI8)
    {
        c0 = ' ';
        c1 = 'v';
    }
    else if (angle > 5 * PI8 && angle <= 7 * PI8)
    {
        c0 = '<';
        c1 = '/';
    }
    else if (angle > 7 * PI8 || angle <= -7 * PI8)
    {
        c0 = '<';
        c1 = '-';
    }
    else if (angle > -7 * PI8 && angle <= -5 * PI8)
    {
        c0 = '<';
        c1 = '\\';
    }
    else if (angle > -5 * PI8 && angle <= -3 * PI8)
    {
        c0 = ' ';
        c1 = '^';
    }
    else
    {
        c0 = '/';
        c1 = '>';
    }
}

void DrawArrow(int x, int y, float dx, float dy, bool erase)
{
    char c0;
    char c1;

    ArrowGlyph(dx, dy, c0, c1);

    if (erase)
    {
        ClearCell(x, y);

        if (c0 != ' ')
        {
            ClearCell(x + 1, y);
        }

        return;
    }

    SetColor(Color::CYAN);

    if (c0 == ' ')
    {
        GotoXY(x, y);
        std::cout << c1;
    }
    else
    {
        GotoXY(x, y);
        std::cout << c0;

        GotoXY(x + 1, y);
        std::cout << c1;
    }

    SetColor();
}

void DrawAmmoHUD()
{
    GotoXY(AMMO_HUD_X, AMMO_HUD_Y);

    SetColor(Color::LIGHT_YELLOW);
    std::cout << "Arrows: ";

    SetColor(Color::CYAN);
    for (int i = 0; i < ammo; ++i)
    {
        std::cout << '^';
    }

    SetColor(Color::GRAY);
    for (int i = ammo; i < MAX_AMMO; ++i)
    {
        std::cout << '.';
    }

    SetColor();
}

bool CheckHit(int arrowX, int arrowY, int tgtX, int tgtY)
{
    int headX = arrowX + 1;

    return (headX >= tgtX && headX <= tgtX + 2) &&
        (arrowY >= tgtY - 1 && arrowY <= tgtY + 1);
}

void UpdateArrows()
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active) continue;

        int oldX = (int)arrows[i].x;
        int oldY = (int)arrows[i].y;

        arrows[i].prevX = arrows[i].x;
        arrows[i].prevY = arrows[i].y;

        arrows[i].x += ARROW_SPEED;

        int newX = (int)arrows[i].x;
        int newY = (int)arrows[i].y;

        float dx = arrows[i].x - arrows[i].prevX;
        float dy = arrows[i].y - arrows[i].prevY;

        if (fabsf(dx) < 0.001f && fabsf(dy) < 0.001f)
        {
            dx = 1.0f;
        }

        if (newY < 1 || newY >= CONSOLE_HEIGHT - 1)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;
            continue;
        }

        int hitIndex = FindHitTarget(newX, newY);

        if (hitIndex != -1)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;

            StartTargetBlink(hitIndex);
            DestroyTarget(hitIndex);

            ShakeConsoleWindow(6, 300, 20);
            continue;
        }

        if (newX >= CONSOLE_WIDTH - 1)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;
            continue;
        }

        if (newX != oldX || newY != oldY)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
        }

        DrawArrow(newX, newY, dx, dy);
    }
}