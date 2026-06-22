#pragma once
#include "Game.h"

constexpr int ARROW_BASE_SPEED = 2;
constexpr int MAX_ARROWS = 16;
constexpr int MAX_AMMO = 10;
constexpr int AMMO_HUD_X = 2;
constexpr int AMMO_HUD_Y = CONSOLE_HEIGHT - 2;

extern float arrowSpeedMultiplier;   // 스킬: 화살 이동 스피드 배수 (기본 1.0f)

struct Arrow
{
    float x;
    float y;
    bool  active;
};

extern Arrow arrows[MAX_ARROWS];
extern int   ammo;

void SpawnArrow(int startX, int startY);
void DrawArrow(int x, int y, bool erase = false);
void DrawAmmoHUD();
bool CheckHit(int arrowX, int arrowY, int tgtX, int tgtY);
void UpdateArrows();