#pragma once
#include "Game.h"

constexpr int ARROW_SPEED = 2;
constexpr int MAX_ARROWS = 16;
constexpr int MAX_AMMO = 10;
constexpr int AMMO_HUD_X = 2;
constexpr int AMMO_HUD_Y = CONSOLE_HEIGHT - 2;

struct Arrow
{
    float x;
    float y;
    float prevX; // 방향 계산용 이전 위치
    float prevY;
    bool  active;
};

extern Arrow arrows[MAX_ARROWS];
extern int   ammo;

void SpawnArrow(int startX, int startY);
void DrawArrow(int x, int y, float dx, float dy, bool erase = false);
void DrawAmmoHUD();
bool CheckHit(int arrowX, int arrowY, int tgtX, int tgtY);
void UpdateArrows();