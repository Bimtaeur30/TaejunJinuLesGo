#pragma once
#include "Game.h"
#include "Target.h"   // MAX_STAGE 사용

// ==================== 장애물 설정 ====================
// 장애물 수 = stage - 1 (1스테이지=0개, 5스테이지=4개)
constexpr int MAX_OBSTACLES = MAX_STAGE - 1;

// 장애물은 과녁보다 왼쪽(화살이 날아가는 중간 경로)에 배치
constexpr int OBSTACLE_AREA_LEFT = 20;
constexpr int OBSTACLE_AREA_RIGHT = CONSOLE_WIDTH - 30;

constexpr int OBSTACLE_MIN_Y = 3;
constexpr int OBSTACLE_MAX_Y = CONSOLE_HEIGHT - 5;

constexpr float OBSTACLE_BASE_SPEED = 0.10f;
constexpr float OBSTACLE_SPEED_STEP = 0.05f;

// 과녁과 동일하게 3칸(상/중/하)을 차지 → 겹침 판정에 사용할 최소 Y 간격
constexpr int OBSTACLE_MIN_GAP_Y = 4;

struct Obstacle
{
    float y;
    int   x;
    int   dir;          // +1 아래, -1 위
    float speed;
    int   prevDrawY;    // 마지막으로 화면에 그린 Y
    bool  active;
    bool  hidden;        // true면 스킬 효과로 화면에서 숨겨지고 충돌/이동 없음
};

extern Obstacle obstacles[MAX_OBSTACLES];
extern int      obstacleCount;

void InitObstacles(int stage);
void DrawObstacle(int idx, bool erase = false);
void UpdateObstacles();
bool CheckObstacleHit(int arrowX, int arrowY, int obsX, int obsY);
void SetObstaclesHidden(bool hidden);   // 스킬: 일시적으로 장애물 전체 숨김/복귀