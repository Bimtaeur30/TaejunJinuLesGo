#include "Arrow.h"
#include "Target.h"
#include <cmath>

Arrow arrows[MAX_ARROWS] = {};
int   ammo = MAX_AMMO;

void SpawnArrow(int startX, int startY)
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active)
        {
            float fx = (float)startX + 1;
            float fy = (float)startY;
            arrows[i] = { fx, fy, fx, fy, true };
            return;
        }
    }
}

// 이동 벡터 (dx, dy) → 8방향 화살 문자 2칸
static void ArrowGlyph(float dx, float dy, char& c0, char& c1)
{
    float ax = dx;
    float ay = dy * 0.5f;
    float angle = atan2f(ay, ax); // -PI ~ PI, 오른쪽=0

    const float P = 3.14159f;
    const float PI8 = P / 8.0f;

    if (angle > -PI8 && angle <= PI8) { c0 = '-';  c1 = '>'; } // E
    else if (angle > PI8 && angle <= 3 * PI8) { c0 = '\\'; c1 = '>'; } // SE
    else if (angle > 3 * PI8 && angle <= 5 * PI8) { c0 = ' ';  c1 = 'v'; } // S
    else if (angle > 5 * PI8 && angle <= 7 * PI8) { c0 = '<';  c1 = '/'; } // SW
    else if (angle > 7 * PI8 || angle <= -7 * PI8) { c0 = '<';  c1 = '-'; } // W
    else if (angle > -7 * PI8 && angle <= -5 * PI8) { c0 = '<';  c1 = '\\'; } // NW
    else if (angle > -5 * PI8 && angle <= -3 * PI8) { c0 = ' ';  c1 = '^'; } // N
    else { c0 = '/';  c1 = '>'; } // NE
}

void DrawArrow(int x, int y, float dx, float dy, bool erase)
{
    char c0, c1;
    ArrowGlyph(dx, dy, c0, c1);

    if (erase)
    {
        ClearCell(x, y);
        if (c0 != ' ') ClearCell(x + 1, y);
        return;
    }

    SetColor(Color::CYAN);
    if (c0 == ' ')
    {
        GotoXY(x, y); cout << c1;
    }
    else
    {
        GotoXY(x, y);     cout << c0;
        GotoXY(x + 1, y); cout << c1;
    }
    SetColor();
}

void DrawAmmoHUD()
{
    GotoXY(AMMO_HUD_X, AMMO_HUD_Y);

    SetColor(Color::LIGHT_YELLOW);
    cout << "Arrows: ";

    SetColor(Color::CYAN);
    for (int i = 0; i < ammo; ++i)
        cout << '^';

    SetColor(Color::GRAY);
    for (int i = ammo; i < MAX_AMMO; ++i)
        cout << '.';

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
    int tgtY = (int)targetY;

    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active) continue;

        int oldX = (int)arrows[i].x;
        int oldY = (int)arrows[i].y;

        arrows[i].prevX = arrows[i].x;
        arrows[i].prevY = arrows[i].y;

        // X 이동 (직선)
        arrows[i].x += ARROW_SPEED;

        int newX = (int)arrows[i].x;
        int newY = (int)arrows[i].y;

        // 이동 벡터 (방향 계산용)
        float dx = arrows[i].x - arrows[i].prevX;
        float dy = arrows[i].y - arrows[i].prevY;
        if (fabsf(dx) < 0.001f && fabsf(dy) < 0.001f) dx = 1.0f; // 기본 오른쪽

        // 화면 위아래 이탈
        if (newY < 1 || newY >= CONSOLE_HEIGHT - 1)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;
            continue;
        }

        // 과녁 충돌
        if (CheckHit(newX, newY, TARGET_X, tgtY))
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;
            StartTargetBlink();
            ShakeConsoleWindow(6, 300, 20);
            continue;
        }

        // 오른쪽 벽 이탈
        if (newX >= CONSOLE_WIDTH - 1)
        {
            DrawArrow(oldX, oldY, dx, dy, true);
            arrows[i].active = false;
            continue;
        }

        if (newX != oldX || newY != oldY)
            DrawArrow(oldX, oldY, dx, dy, true);

        DrawArrow(newX, newY, dx, dy);
    }
}