#pragma once
#include "Game.h"
#include "Skill.h"

// ==================== 카드 레이아웃 ====================
constexpr int CARD_COUNT = 3;
constexpr int CARD_WIDTH = 13;
constexpr int CARD_HEIGHT = 7;
constexpr int CARD_GAP = 3;

// ==================== 애니메이션 ====================
constexpr int CARD_SLIDE_FRAMES = 14;   // 아래→중간으로 올라오는 데 걸리는 프레임 수

// 카드 선택 단계를 실행한다.
// 카드 3장을 무작위 스킬로 채우고, 아래에서 중앙으로 슬라이드시킨 뒤
// 좌/우로 고른 카드를 스페이스/엔터로 확정하면 해당 스킬을 슬롯에 추가하고 리턴한다.
// 스킬 슬롯이 이미 가득 찬 경우(MAX_SKILL_SLOTS 초과) 카드 단계를 건너뛴다.
void RunCardSelection();