#include "Skill.h"
#include "Obstacle.h"
#include "Bow.h"
#include "Arrow.h"
#include <cstdio>

SkillSlot skillSlots[MAX_SKILL_SLOTS] = {};
int        skillSlotCount = 0;

// ───────────────────────────────────────────────
//  스킬 박스 UI 위치 (우측 하단)
// ───────────────────────────────────────────────
constexpr int SKILL_BOX_WIDTH = 14;
constexpr int SKILL_BOX_X = CONSOLE_WIDTH - SKILL_BOX_WIDTH - 2;
constexpr int SKILL_BOX_Y = CONSOLE_HEIGHT - 2 - MAX_SKILL_SLOTS;

const char* GetSkillName(SkillType type)
{
    switch (type)
    {
    case SkillType::CLEAR_OBSTACLES: return "Clear Obs";
    case SkillType::BOW_SPEED_UP:    return "Bow Speed";
    case SkillType::ARROW_SPEED_UP:  return "Arrow Spd";
    default: return "?";
    }
}

static float GetSkillDuration(SkillType type)
{
    switch (type)
    {
    case SkillType::CLEAR_OBSTACLES: return SKILL_DURATION_OBSTACLE;
    case SkillType::BOW_SPEED_UP:    return SKILL_DURATION_BOW_SPEED;
    case SkillType::ARROW_SPEED_UP:  return SKILL_DURATION_ARROW_SPEED;
    default: return 0.0f;
    }
}

// ───────────────────────────────────────────────
//  슬롯 초기화 (게임/스테이지1 시작 시 1회)
// ───────────────────────────────────────────────
void InitSkills()
{
    for (int i = 0; i < MAX_SKILL_SLOTS; ++i)
        skillSlots[i] = {};
    skillSlotCount = 0;

    bowSpeedMultiplier = 1.0f;
    arrowSpeedMultiplier = 1.0f;
    SetObstaclesHidden(false);
}

// ───────────────────────────────────────────────
//  새 스킬을 슬롯에 추가 (카드 선택 결과)
// ───────────────────────────────────────────────
bool AddSkill(SkillType type)
{
    if (skillSlotCount >= MAX_SKILL_SLOTS) return false;

    skillSlots[skillSlotCount].type = type;
    skillSlots[skillSlotCount].acquired = true;
    skillSlots[skillSlotCount].cooldownLeft = 0.0f;
    skillSlots[skillSlotCount].effectLeft = 0.0f;
    ++skillSlotCount;
    return true;
}

// ───────────────────────────────────────────────
//  효과 시작 (실제 게임플레이에 적용)
// ───────────────────────────────────────────────
static void ActivateEffect(SkillType type)
{
    switch (type)
    {
    case SkillType::CLEAR_OBSTACLES:
        SetObstaclesHidden(true);
        break;
    case SkillType::BOW_SPEED_UP:
        bowSpeedMultiplier = SKILL_BOW_SPEED_MULTIPLIER;
        break;
    case SkillType::ARROW_SPEED_UP:
        arrowSpeedMultiplier = SKILL_ARROW_SPEED_MULTIPLIER;
        break;
    default: break;
    }
}

// ───────────────────────────────────────────────
//  효과 종료 (원상복구)
// ───────────────────────────────────────────────
static void DeactivateEffect(SkillType type)
{
    switch (type)
    {
    case SkillType::CLEAR_OBSTACLES:
        SetObstaclesHidden(false);
        break;
    case SkillType::BOW_SPEED_UP:
        bowSpeedMultiplier = 1.0f;
        break;
    case SkillType::ARROW_SPEED_UP:
        arrowSpeedMultiplier = 1.0f;
        break;
    default: break;
    }
}

// ───────────────────────────────────────────────
//  1~4 키 입력으로 슬롯 사용 시도
// ───────────────────────────────────────────────
void TryUseSkill(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= skillSlotCount) return;

    SkillSlot& s = skillSlots[slotIndex];
    if (!s.acquired)        return;
    if (s.cooldownLeft > 0) return;   // 쿨타임 중
    if (s.effectLeft > 0)   return;   // 이미 효과 발동 중

    s.effectLeft = GetSkillDuration(s.type);
    s.cooldownLeft = SKILL_COOLDOWN_SEC;
    ActivateEffect(s.type);
}

// ───────────────────────────────────────────────
//  매 프레임 호출 — 지속시간/쿨타임 카운트다운
// ───────────────────────────────────────────────
void UpdateSkills(float deltaSeconds)
{
    for (int i = 0; i < skillSlotCount; ++i)
    {
        SkillSlot& s = skillSlots[i];
        if (!s.acquired) continue;

        if (s.effectLeft > 0)
        {
            s.effectLeft -= deltaSeconds;
            if (s.effectLeft <= 0)
            {
                s.effectLeft = 0;
                DeactivateEffect(s.type);
            }
        }

        if (s.cooldownLeft > 0)
        {
            s.cooldownLeft -= deltaSeconds;
            if (s.cooldownLeft < 0) s.cooldownLeft = 0;
        }
    }
}

// ───────────────────────────────────────────────
//  우측 하단 스킬 박스 UI
// ───────────────────────────────────────────────
void DrawSkillBox()
{
    // 박스 테두리
    SetColor(Color::LIGHT_GRAY);
    GotoXY(SKILL_BOX_X - 1, SKILL_BOX_Y - 1);
    cout << '+';
    for (int x = 0; x < SKILL_BOX_WIDTH; ++x) cout << '-';
    cout << '+';

    for (int row = 0; row < MAX_SKILL_SLOTS; ++row)
    {
        GotoXY(SKILL_BOX_X - 1, SKILL_BOX_Y + row);
        SetColor(Color::LIGHT_GRAY);
        cout << '|';

        char buf[32];

        if (row < skillSlotCount && skillSlots[row].acquired)
        {
            SkillSlot& s = skillSlots[row];

            if (s.effectLeft > 0)
            {
                SetColor(Color::LIGHT_GREEN);
                snprintf(buf, sizeof(buf), "[%d]%-9s ON", row + 1, GetSkillName(s.type));
            }
            else if (s.cooldownLeft > 0)
            {
                SetColor(Color::GRAY);
                snprintf(buf, sizeof(buf), "[%d]%-9s%2d", row + 1, GetSkillName(s.type), (int)(s.cooldownLeft + 0.99f));
            }
            else
            {
                SetColor(Color::WHITE);
                snprintf(buf, sizeof(buf), "[%d]%-9s RD", row + 1, GetSkillName(s.type));
            }
        }
        else
        {
            SetColor(Color::GRAY);
            snprintf(buf, sizeof(buf), "[%d] ---------", row + 1);
        }

        cout << buf;

        SetColor(Color::LIGHT_GRAY);
        GotoXY(SKILL_BOX_X - 1 + SKILL_BOX_WIDTH + 1, SKILL_BOX_Y + row);
        cout << '|';
    }

    GotoXY(SKILL_BOX_X - 1, SKILL_BOX_Y + MAX_SKILL_SLOTS);
    SetColor(Color::LIGHT_GRAY);
    cout << '+';
    for (int x = 0; x < SKILL_BOX_WIDTH; ++x) cout << '-';
    cout << '+';

    SetColor();
}