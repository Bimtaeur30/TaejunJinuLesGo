#pragma once
#include "Game.h"

constexpr int   MAX_STAGE = 5;
constexpr int   MAX_TARGETS = MAX_STAGE;

constexpr int   TARGET_AREA_LEFT = CONSOLE_WIDTH - 25;
constexpr int   TARGET_AREA_RIGHT = CONSOLE_WIDTH - 5;

constexpr int   TARGET_MIN_Y = 3;
constexpr int   TARGET_MAX_Y = CONSOLE_HEIGHT - 5;

constexpr int BLINK_TOTAL_FRAMES = 18;  // ~0.6초
constexpr int BLINK_INTERVAL_FRAMES = 3;

constexpr float TARGET_BASE_SPEED = 0.12f;   // 스테이지 1 기본 속도
constexpr float TARGET_SPEED_STEP = 0.06f;   // 스테이지당 증가량

struct Target
{
    float y;
    int   x;
    int   dir;
    float speed;
    int   blinkFrames;
    bool  active;
    bool  removing;
    int   prevDrawY;
};

extern Target targets[MAX_TARGETS];
extern int    targetCount;
extern int    currentStage;

void InitStage(int stage);
void DrawTarget(int idx, bool erase = false);
void StartTargetBlink(int idx);
void UpdateTargets();
bool AllTargetsCleared();
void DrawStageHUD();