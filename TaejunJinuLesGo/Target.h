#pragma once
#include "Game.h"

constexpr int MAX_STAGE = 3;
constexpr int MAX_TARGETS = 5;

constexpr int TARGET_MIN_Y = 3;
constexpr int TARGET_MAX_Y = CONSOLE_HEIGHT - 4;

constexpr int BLINK_TOTAL_FRAMES = 10;
constexpr int BLINK_INTERVAL_FRAMES = 2;

struct Target
{
    int x = 0;
    float y = 0.0f;
    int dir = 1;
    float speed = 0.25f;
    int blinkFrames = 0;
    bool active = false;
};

extern Target targets[MAX_TARGETS];
extern int targetCount;
extern int currentStage;

void InitStage(int stage);
void DrawStageHUD();

void DrawTarget(int index, bool erase = false);
void StartTargetBlink(int index);
void DestroyTarget(int index);
void UpdateTarget();

int FindHitTarget(int arrowX, int arrowY);
bool AreAllTargetsDead();
void UpdateTargets();
bool AllTargetsCleared();