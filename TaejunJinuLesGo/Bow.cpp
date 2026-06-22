#include "Bow.h"
#include "Arrow.h"

int   bowY = CONSOLE_HEIGHT / 2;
float bowSpeedMultiplier = 1.0f;
static int  bowMoveCooldown = 0;
static bool bowMoveHeld = false;

void DrawBow(int x, int y, bool erase)
{
    if (erase)
    {
        for (int dy = -1; dy <= 1; ++dy) ClearCell(x, y + dy);
        ClearCell(x + 1, y);
        ClearCell(x + 2, y);
        return;
    }

    SetColor(Color::LIGHT_YELLOW);
    GotoXY(x, y - 1); cout << ')';
    GotoXY(x, y);     cout << ')';
    GotoXY(x, y + 1); cout << ')';

    SetColor(Color::WHITE);
    GotoXY(x + 1, y); cout << '=';
    GotoXY(x + 2, y); cout << '>';

    SetColor();
}

void HandleInput()
{
    bool upHeld = GetKey(VK_UP);
    bool downHeld = GetKey(VK_DOWN);
    bool anyDir = upHeld || downHeld;

    if (bowMoveCooldown > 0)
        --bowMoveCooldown;

    if (!anyDir)
    {
        bowMoveHeld = false;
        bowMoveCooldown = 0;
    }

    if (anyDir && bowMoveCooldown == 0)
    {
        int dir = upHeld ? -1 : 1;
        int nextY = bowY + dir;

        if (nextY >= BOW_MIN_Y + 1 && nextY <= BOW_MAX_Y - 1)
        {
            DrawBow(BOW_X, bowY, true);
            bowY = nextY;
            DrawBow(BOW_X, bowY);
        }

        int baseDelay = bowMoveHeld ? BOW_MOVE_REPEAT_DELAY : BOW_MOVE_FIRST_DELAY;
        // 배수가 클수록 대기 프레임이 줄어 더 빠르게 이동
        int delay = (int)(baseDelay / bowSpeedMultiplier);
        if (delay < 1) delay = 1;

        bowMoveCooldown = delay;
        bowMoveHeld = true;
    }

    if (GetKeyDown(VK_SPACE) && ammo > 0)
    {
        SpawnArrow(BOW_X + 2, bowY);
        --ammo;
        DrawAmmoHUD();
        ShakeConsoleWindow(2, 80, 20);
    }
}