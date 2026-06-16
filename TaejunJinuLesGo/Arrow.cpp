#include "Arrow.h"
#include "Target.h"

Arrow arrows[MAX_ARROWS] = {};
int   ammo = MAX_AMMO;

void SpawnArrow(int startX, int startY)
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active)
        {
            arrows[i] = { (float)startX, (float)startY, true };
            return;
        }
    }
}

void DrawArrow(int x, int y, bool erase)
{
    if (erase)
    {
        ClearCell(x, y);
        ClearCell(x + 1, y);
        return;
    }

    SetColor(Color::CYAN);
    GotoXY(x, y); cout << '-';
    GotoXY(x + 1, y); cout << '>';
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
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active) continue;

        int oldX = (int)arrows[i].x;
        int oldY = (int)arrows[i].y;

        arrows[i].x += ARROW_SPEED;

        int newX = (int)arrows[i].x;
        int newY = (int)arrows[i].y;

        if (newX >= CONSOLE_WIDTH - 1)
        {
            DrawArrow(oldX, oldY, true);
            arrows[i].active = false;
            continue;
        }

        // 과녁 여러개 충돌 검사
        bool hit = false;
        for (int t = 0; t < targetCount; ++t)
        {
            if (!targets[t].active)  continue;
            if (targets[t].removing) continue;

            if (CheckHit(newX, newY, targets[t].x, (int)targets[t].y))
            {
                DrawArrow(oldX, oldY, true);
                arrows[i].active = false;
                StartTargetBlink(t);
                ShakeConsoleWindow(6, 300, 20);
                hit = true;
                break;
            }
        }
        if (hit) continue;

        if (newX != oldX)
            DrawArrow(oldX, oldY, true);

        DrawArrow(newX, newY);
    }
}