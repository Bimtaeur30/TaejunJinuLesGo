#include "Obstacle.h"
#include <cstdlib>

Obstacle obstacles[MAX_OBSTACLES] = {};
int      obstacleCount = 0;

// ───────────────────────────────────────────────
//  지정 좌표(x, y)의 장애물 3칸을 화면에서 지운다
// ───────────────────────────────────────────────
static void EraseObstacleAt(int x, int y)
{
    for (int dy = -1; dy <= 1; ++dy)
    {
        ClearCell(x, y + dy);
        ClearCell(x + 1, y + dy);
        ClearCell(x + 2, y + dy);
    }
}

// ───────────────────────────────────────────────
//  장애물 초기화
//  stage - 1 개 생성, X는 영역 내 균등 분배, Y는 서로 겹치지 않게 분산
// ───────────────────────────────────────────────
void InitObstacles(int stage)
{
    obstacleCount = stage - 1;
    if (obstacleCount < 0) obstacleCount = 0;
    if (obstacleCount > MAX_OBSTACLES) obstacleCount = MAX_OBSTACLES;

    for (int i = 0; i < MAX_OBSTACLES; ++i)
        obstacles[i] = {};

    if (obstacleCount == 0) return;

    int areaWidth = OBSTACLE_AREA_RIGHT - OBSTACLE_AREA_LEFT;
    int slotX = (obstacleCount > 1) ? areaWidth / (obstacleCount - 1) : 0;

    // Y 분산: 가용 구간을 obstacleCount등분 + 칸마다 최소 간격 보장
    int rangeY = OBSTACLE_MAX_Y - OBSTACLE_MIN_Y;
    int slotY = (obstacleCount > 0) ? rangeY / obstacleCount : rangeY;
    if (slotY < OBSTACLE_MIN_GAP_Y) slotY = OBSTACLE_MIN_GAP_Y;

    for (int i = 0; i < obstacleCount; ++i)
    {
        int xPos = (obstacleCount == 1)
            ? (OBSTACLE_AREA_LEFT + areaWidth / 2)
            : (OBSTACLE_AREA_LEFT + slotX * i);

        // 각 장애물을 자기 슬롯 구간 내에서 약간 랜덤하게 시작 (겹침 없이)
        int slotStart = OBSTACLE_MIN_Y + slotY * i;
        int jitterY = (slotY > 1) ? (rand() % (slotY - 1)) : 0;
        int startY = slotStart + jitterY;
        if (startY > OBSTACLE_MAX_Y) startY = OBSTACLE_MAX_Y;
        if (startY < OBSTACLE_MIN_Y) startY = OBSTACLE_MIN_Y;

        float base = OBSTACLE_BASE_SPEED + OBSTACLE_SPEED_STEP * (stage - 2);
        float jitter = ((float)(rand() % 9) - 4) * 0.012f;
        float spd = base + jitter;
        if (spd < 0.04f) spd = 0.04f;

        obstacles[i].y = (float)startY;
        obstacles[i].x = xPos;
        obstacles[i].dir = (i % 2 == 0) ? 1 : -1;
        obstacles[i].speed = spd;
        obstacles[i].prevDrawY = startY;
        obstacles[i].active = true;
        obstacles[i].hidden = false;
    }
}

// ───────────────────────────────────────────────
//  장애물 그리기
// ───────────────────────────────────────────────
void DrawObstacle(int idx, bool erase)
{
    if (idx < 0 || idx >= obstacleCount) return;

    Obstacle& o = obstacles[idx];
    int x = o.x;
    int y = (int)o.y;

    if (erase)
    {
        EraseObstacleAt(x, y);
        return;
    }

    for (int dy = -1; dy <= 1; ++dy)
    {
        GotoXY(x, y + dy);
        SetColor(Color::GRAY);
        cout << '[';
        SetColor(Color::WHITE);
        cout << (dy == 0 ? '#' : 'X');
        SetColor(Color::GRAY);
        cout << ']';
    }
    SetColor();

    o.prevDrawY = y;
}

// ───────────────────────────────────────────────
//  두 장애물(인덱스 a, b 제외)이 Y축 기준 겹치는지 확인
//  과녁과 동일한 룰: 최소 간격(OBSTACLE_MIN_GAP_Y) 미만이면 충돌로 간주
// ───────────────────────────────────────────────
static bool WouldOverlap(int selfIdx, int x, float candidateY)
{
    for (int j = 0; j < obstacleCount; ++j)
    {
        if (j == selfIdx) continue;
        if (!obstacles[j].active) continue;
        if (obstacles[j].x != x) continue;   // X가 다르면 겹침 검사 불필요

        float diff = candidateY - obstacles[j].y;
        if (diff < 0) diff = -diff;
        if (diff < OBSTACLE_MIN_GAP_Y) return true;
    }
    return false;
}

// ───────────────────────────────────────────────
//  전체 장애물 업데이트 — 과녁처럼 상하 이동, 서로 겹치지 않도록 방향 반전
// ───────────────────────────────────────────────
void UpdateObstacles()
{
    for (int i = 0; i < obstacleCount; ++i)
    {
        Obstacle& o = obstacles[i];
        if (!o.active) continue;
        if (o.hidden)  continue;   // 스킬로 숨겨진 동안은 이동/그리기 없음

        int drawnY = o.prevDrawY;

        float nextY = o.y + o.speed * o.dir;

        // 화면 경계 반사
        if (nextY <= OBSTACLE_MIN_Y) { nextY = (float)OBSTACLE_MIN_Y; o.dir = 1; }
        if (nextY >= OBSTACLE_MAX_Y) { nextY = (float)OBSTACLE_MAX_Y; o.dir = -1; }

        // 다른 장애물과 겹칠 것 같으면 방향을 반전하고 그대로 멈춤(이번 프레임은 이동 보류)
        if (WouldOverlap(i, o.x, nextY))
        {
            o.dir = -o.dir;
        }
        else
        {
            o.y = nextY;
        }

        int newY = (int)o.y;

        if (newY != drawnY)
        {
            EraseObstacleAt(o.x, drawnY);
            DrawObstacle(i);
        }
    }
}

// ───────────────────────────────────────────────
//  스킬: 장애물 전체를 일시적으로 숨기거나 복귀시킨다
// ───────────────────────────────────────────────
void SetObstaclesHidden(bool hidden)
{
    for (int i = 0; i < obstacleCount; ++i)
    {
        Obstacle& o = obstacles[i];
        if (!o.active) continue;

        if (hidden && !o.hidden)
        {
            EraseObstacleAt(o.x, o.prevDrawY);
            o.hidden = true;
        }
        else if (!hidden && o.hidden)
        {
            o.hidden = false;
            DrawObstacle(i);   // 원래 위치에 다시 그리기 (prevDrawY 갱신)
        }
    }
}

// ───────────────────────────────────────────────
//  화살-장애물 충돌 판정 (과녁과 동일한 3칸 판정)
// ───────────────────────────────────────────────
bool CheckObstacleHit(int arrowX, int arrowY, int obsX, int obsY)
{
    int headX = arrowX + 1;
    return (headX >= obsX && headX <= obsX + 2) &&
        (arrowY >= obsY - 1 && arrowY <= obsY + 1);
}