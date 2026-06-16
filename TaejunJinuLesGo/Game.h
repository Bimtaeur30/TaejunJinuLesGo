#pragma once
#include "Console.h"

constexpr int CONSOLE_WIDTH = 80;
constexpr int CONSOLE_HEIGHT = 30;
constexpr int FPS = 30;

// 공유 유틸리티
void ClearCell(int x, int y);
void DrawBorder();

// 게임 흐름
void Init();
void RunGame();
void StartStage(int stage);
void ShowStageClear(int stage);
void ShowGameClear();