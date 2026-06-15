#pragma once
#include "Game.h"

constexpr int BOW_X = 3;
constexpr int BOW_MIN_Y = 1;
constexpr int BOW_MAX_Y = CONSOLE_HEIGHT - 3;
constexpr int BOW_MOVE_FIRST_DELAY = 8;
constexpr int BOW_MOVE_REPEAT_DELAY = 3;

extern int bowY;

void DrawBow(int x, int y, bool erase = false);
void HandleInput();