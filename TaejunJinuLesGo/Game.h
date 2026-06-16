#pragma once
#include "Console.h"

constexpr int CONSOLE_WIDTH = 80;
constexpr int CONSOLE_HEIGHT = 30;
constexpr int FPS = 30;

// 공유 유틸리티
void ClearCell(int x, int y);
void DrawBorder();

// 초기화 / 스테이지 전환
void Init();
void StartStage(int stage);
void ShowStageClear(int stage);
void ShowGameClear();