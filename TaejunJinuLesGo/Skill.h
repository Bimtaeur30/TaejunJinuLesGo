#pragma once
#include "Game.h"

// ==================== 스킬 종류 ====================
enum class SkillType
{
    CLEAR_OBSTACLES,   // 1. 3초 동안 장애물 없애기
    BOW_SPEED_UP,      // 2. 5초 동안 활 이동 스피드 증가
    ARROW_SPEED_UP,    // 3. 5초 동안 화살 이동 스피드 증가

    COUNT
};

constexpr int SKILL_TYPE_COUNT = (int)SkillType::COUNT;

// ==================== 스킬 수치 ====================
constexpr float SKILL_DURATION_OBSTACLE = 3.0f;   // 초
constexpr float SKILL_DURATION_BOW_SPEED = 5.0f;
constexpr float SKILL_DURATION_ARROW_SPEED = 5.0f;

constexpr float SKILL_COOLDOWN_SEC = 5.0f;          // 모든 스킬 공통 쿨타임

constexpr float SKILL_BOW_SPEED_MULTIPLIER = 2.0f;
constexpr float SKILL_ARROW_SPEED_MULTIPLIER = 2.0f;

// ==================== 보유 슬롯 ====================
constexpr int MAX_SKILL_SLOTS = 4;   // 동시에 보유 가능한 스킬 수 (1,2,3,4 키)

struct SkillSlot
{
    SkillType type;
    bool      acquired;        // 이 슬롯이 사용 중인지
    float     cooldownLeft;    // 남은 쿨타임(초). 0이면 사용 가능
    float     effectLeft;      // 지속효과 남은 시간(초). 0이면 효과 종료 상태
};

extern SkillSlot skillSlots[MAX_SKILL_SLOTS];
extern int        skillSlotCount;   // 현재까지 획득한 스킬 수

// ==================== 함수 ====================
void   InitSkills();                       // 게임 시작 시 슬롯 초기화
bool   AddSkill(SkillType type);            // 새 스킬을 슬롯에 추가 (순서대로 1,2,3,4)
void   TryUseSkill(int slotIndex);          // 1~4 키 입력 시 호출 (slotIndex: 0-based)
void   UpdateSkills(float deltaSeconds);    // 매 프레임 쿨타임/지속시간 갱신
void   DrawSkillBox();                      // 우측 하단 스킬 박스 UI 그리기
const char* GetSkillName(SkillType type);