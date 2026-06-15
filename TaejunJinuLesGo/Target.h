#pragma once
#include "Game.h"

constexpr int   TARGET_X = CONSOLE_WIDTH - 8;
constexpr int   TARGET_MIN_Y = 2;
constexpr int   TARGET_MAX_Y = CONSOLE_HEIGHT - 4;
constexpr float TARGET_SPEED = 0.15f;

constexpr int BLINK_TOTAL_FRAMES = 18; // 블링크 지속 프레임 (30fps 기준 ~0.6초)
constexpr int BLINK_INTERVAL_FRAMES = 3;  // 흰색 <-> 원래색 전환 간격

extern float targetY;
extern int   targetDir;
extern int   targetBlinkFrames; // 0이면 블링크 없음

void DrawTarget(int x, int y, bool erase = false);
void StartTargetBlink();
void UpdateTarget();