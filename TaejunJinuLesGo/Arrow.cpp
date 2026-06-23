#include "Arrow.h"
#include "Target.h"
#include "Obstacle.h"

Arrow arrows[MAX_ARROWS] = {};
int   ammo = MAX_AMMO;
float arrowSpeedMultiplier = 1.0f;

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

        arrows[i].x += ARROW_BASE_SPEED * arrowSpeedMultiplier;

        int newX = (int)arrows[i].x;
        int newY = (int)arrows[i].y;

        // 화면 밖 이탈
        if (newX >= CONSOLE_WIDTH - 1)
        {
            DrawArrow(oldX, oldY, true);
            arrows[i].active = false;
            continue;
        }

        // ── 연속 충돌 검사 ──────────────────────────────
        // 속도가 빨라지면 한 프레임에 여러 칸을 건너뛰므로,
        // oldX+1 ~ newX (화살 머리가 지나간 모든 X)를 한 칸씩 검사한다.
        bool hit = false;
        bool blocked = false;

        for (int scanX = oldX + 1; scanX <= newX; ++scanX)
        {
            // 과녁 검사
            for (int t = 0; t < targetCount; ++t)
            {
                if (!targets[t].active)  continue;
                if (targets[t].removing) continue;

                if (CheckHit(scanX, newY, targets[t].x, (int)targets[t].y))
                {
                    hit = true;
                    DrawArrow(oldX, oldY, true);
                    arrows[i].active = false;
                    StartTargetBlink(t);
                    ShakeConsoleWindow(6, 300, 20);
                    break;
                }
            }
            if (hit) break;

            // 장애물 검사 (화살만 소멸, 장애물은 파괴되지 않음
            for (int o = 0; o < obstacleCount; ++o)
            {
                if (!obstacles[o].active) continue;
                if (obstacles[o].hidden)  continue;   // 스킬로 숨겨진 동안은 충돌 없음

                if (CheckObstacleHit(scanX, newY, obstacles[o].x, (int)obstacles[o].y))
                {
                    blocked = true;
                    DrawArrow(oldX, oldY, true);
                    arrows[i].active = false;
                    ShakeConsoleWindow(4, 200, 15);
                    break;
                }
            }
            if (blocked) break;
        }
        // ─────────────────────────────────────────────────

        if (hit || blocked) continue;

        if (newX != oldX)
            DrawArrow(oldX, oldY, true);

        DrawArrow(newX, newY);
    }
}