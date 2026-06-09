#include "Console.h"

constexpr int CONSOLE_WIDTH = 80;
constexpr int CONSOLE_HEIGHT = 30;
constexpr int FPS = 30;

constexpr int BOW_X = 3;
constexpr int BOW_MIN_Y = 1;
constexpr int BOW_MAX_Y = CONSOLE_HEIGHT - 3;

constexpr int ARROW_SPEED = 2;

constexpr int BOW_MOVE_FIRST_DELAY = 8;
constexpr int BOW_MOVE_REPEAT_DELAY = 3;

struct Arrow
{
    float x;
    int   y;
    bool  active;
};

constexpr int MAX_ARROWS = 16;
constexpr int MAX_AMMO = 10;
constexpr int AMMO_HUD_X = 2;
constexpr int AMMO_HUD_Y = CONSOLE_HEIGHT - 2;

static int   bowY = CONSOLE_HEIGHT / 2;
static Arrow arrows[MAX_ARROWS] = {};
static int   ammo = MAX_AMMO;
static int   bowMoveCooldown = 0;
static bool  bowMoveHeld = false;

void SpawnArrow(int startX, int startY)
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active)
        {
            arrows[i] = { (float)startX + 1, startY, true };
            return;
        }
    }
}

void ClearCell(int x, int y)
{
    if (x < 1 || x >= CONSOLE_WIDTH - 1) return;
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

void DrawBow(int x, int y, bool erase = false)
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

void DrawArrow(int x, int y, bool erase = false)
{
    if (erase) { ClearCell(x, y); ClearCell(x + 1, y); return; }
    SetColor(Color::CYAN);
    GotoXY(x, y);
    cout << '-';
    GotoXY(x + 1, y);
    cout << '>';
    SetColor();
}

void Init()
{
    SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
    SetConsoleWindowStyle(true);
    SetCursorVisible(false);
    SetConsoleGameTitle(L"Arrow Shooting Game");
    SetConsoleMouseInputDisabled();

    system("cls");

    DrawBorder();
    DrawAmmoHUD();
    DrawBow(BOW_X, bowY);
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

        bowMoveCooldown = bowMoveHeld ? BOW_MOVE_REPEAT_DELAY : BOW_MOVE_FIRST_DELAY;
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

void UpdateArrows()
{
    for (int i = 0; i < MAX_ARROWS; ++i)
    {
        if (!arrows[i].active) continue;

        int oldX = (int)arrows[i].x;
        arrows[i].x += ARROW_SPEED;
        int newX = (int)arrows[i].x;

        if (newX >= CONSOLE_WIDTH - 1)
        {
            DrawArrow(oldX, arrows[i].y, true);
            arrows[i].active = false;
            continue;
        }

        if (newX != oldX)
            DrawArrow(oldX, arrows[i].y, true);

        DrawArrow(newX, arrows[i].y);
    }
}

int main()
{
    Init();

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE)) break;

        HandleInput();
        UpdateArrows();

        FrameSync(FPS);
    }

    system("cls");
    SetCursorVisible(true);
    GotoXY(0, 0);
    cout << "Game Over!" << endl;

    return 0;
}