#pragma once
#include "Console.h"

constexpr int CONSOLE_WIDTH = 80;
constexpr int CONSOLE_HEIGHT = 30;
constexpr int FPS = 30;

void ClearCell(int x, int y);
void DrawBorder();

void Init();
void StartStage(int stage);
void ShowStageClear(int stage);
void ShowGameClear();