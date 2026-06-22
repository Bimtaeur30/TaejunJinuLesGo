#include "Card.h"
#include <cstdlib>
#include <cstring>

// 카드 한 장의 화면 좌표 (선택 인덱스 idx: 0,1,2 / 왼쪽부터)
static int CardX(int idx)
{
    int totalWidth = CARD_COUNT * CARD_WIDTH + (CARD_COUNT - 1) * CARD_GAP;
    int startX = (CONSOLE_WIDTH - totalWidth) / 2;
    return startX + idx * (CARD_WIDTH + CARD_GAP);
}

static int CardFinalY()
{
    return (CONSOLE_HEIGHT - CARD_HEIGHT) / 2;
}

// 카드가 화면 밖으로 나가지 않도록 시작 Y를 테두리 바로 안쪽으로 제한.
// (예전 버전은 startY = CONSOLE_HEIGHT로 두어 카드가 콘솔 버퍼 밖으로 그려지면서
//  화면이 밀리고 잔상이 남는 버그가 있었음 — 반드시 보더 안쪽에서 시작해야 함)
static int CardStartY()
{
    return CONSOLE_HEIGHT - 1 - CARD_HEIGHT;
}

// 스킬 종류별 1글자 아이콘 (원 안에 들어갈 표시)
static char GetSkillIcon(SkillType type)
{
    switch (type)
    {
    case SkillType::CLEAR_OBSTACLES: return 'X';   // 장애물 제거
    case SkillType::BOW_SPEED_UP:    return '>';   // 활 스피드
    case SkillType::ARROW_SPEED_UP:  return '^';   // 화살 스피드
    default: return '?';
    }
}

// 카드 한 장을 지정 좌상단(x, y)에 그린다. selected면 강조 테두리.
static void DrawCard(int x, int y, SkillType type, bool selected)
{
    Color lineColor = selected ? Color::LIGHT_YELLOW : Color::WHITE;

    SetColor(lineColor);
    GotoXY(x, y);
    cout << '+';
    for (int i = 0; i < CARD_WIDTH - 2; ++i) cout << '-';
    cout << '+';

    for (int row = 1; row < CARD_HEIGHT - 1; ++row)
    {
        GotoXY(x, y + row);
        cout << '|';
        GotoXY(x + CARD_WIDTH - 1, y + row);
        cout << '|';
    }

    GotoXY(x, y + CARD_HEIGHT - 1);
    cout << '+';
    for (int i = 0; i < CARD_WIDTH - 2; ++i) cout << '-';
    cout << '+';

    // 원형 아이콘 (간단한 원 모양을 문자로 표현)
    int circleY = y + 2;
    int circleX = x + (CARD_WIDTH - 5) / 2;

    GotoXY(circleX, circleY);     cout << " _ ";
    GotoXY(circleX, circleY + 1); cout << "( " << GetSkillIcon(type) << " )";
    GotoXY(circleX, circleY + 2); cout << " - ";

    // 스킬 이름 (가운데 정렬)
    const char* name = GetSkillName(type);
    int nameLen = (int)strlen(name);
    int innerWidth = CARD_WIDTH - 2;
    int nameX = x + 1 + (innerWidth - nameLen) / 2;
    if (nameX < x + 1) nameX = x + 1;

    GotoXY(nameX, y + CARD_HEIGHT - 2);
    cout << name;

    SetColor();
}

// 카드 영역(테두리 포함)을 정확히 지운다. 화면 범위를 벗어나는 줄은 건너뛴다.
static void EraseCard(int x, int y)
{
    SetColor();
    for (int row = 0; row < CARD_HEIGHT; ++row)
    {
        int py = y + row;
        if (py < 1 || py > CONSOLE_HEIGHT - 2) continue;   // 화면(테두리 안쪽) 밖이면 건너뛴다

        GotoXY(x, py);
        for (int i = 0; i < CARD_WIDTH; ++i) cout << ' ';
    }
    SetColor();
}

// 한 줄 텍스트를 지운다 (길이만큼 공백으로 덮어쓰기)
static void EraseLine(int x, int y, int length)
{
    if (y < 1 || y > CONSOLE_HEIGHT - 2) return;
    SetColor();
    GotoXY(x, y);
    for (int i = 0; i < length; ++i) cout << ' ';
    SetColor();
}

// ───────────────────────────────────────────────
//  카드 선택 단계 진입
// ───────────────────────────────────────────────
void RunCardSelection()
{
    if (skillSlotCount >= MAX_SKILL_SLOTS) return;   // 슬롯이 가득 차면 카드 생략

    // 이전 화면을 완전히 지우고 테두리만 다시 그림
    system("cls");
    DrawBorder();

    // 상단 안내: "스킬 획득하세요" 한 줄만
    const char* title = "스킬 획득하세요";
    int titleX = (CONSOLE_WIDTH - (int)strlen(title) / 2) / 2;  // 한글은 폭 2칸이므로 보정
    int titleLen = (int)strlen(title);
    titleX = (CONSOLE_WIDTH - titleLen) / 2;
    int titleY = CardFinalY() - 3;

    SetColor(Color::WHITE);
    GotoXY(titleX, titleY);
    cout << title;
    SetColor();

    // 랜덤 스킬 3장 뽑기
    SkillType cardTypes[CARD_COUNT];
    for (int i = 0; i < CARD_COUNT; ++i)
        cardTypes[i] = (SkillType)(rand() % SKILL_TYPE_COUNT);

    int finalY = CardFinalY();
    int startY = CardStartY();   // 화면 안쪽(테두리 바로 위)에서 시작 — 화면 밖으로 나가지 않음

    // ── 슬라이드 업 애니메이션 ──────────────────────────
    int prevY = startY;
    for (int frame = 1; frame <= CARD_SLIDE_FRAMES; ++frame)
    {
        int curY = startY - (int)((float)(startY - finalY) * frame / CARD_SLIDE_FRAMES);

        if (curY != prevY)
        {
            for (int i = 0; i < CARD_COUNT; ++i)
                EraseCard(CardX(i), prevY);
        }

        for (int i = 0; i < CARD_COUNT; ++i)
            DrawCard(CardX(i), curY, cardTypes[i], false);

        prevY = curY;
        FrameSync(FPS);
    }

    // ── 선택 입력 루프 ──────────────────────────────────
    int selected = 1;   // 가운데 카드부터 시작
    bool confirmed = false;

    for (int i = 0; i < CARD_COUNT; ++i)
        DrawCard(CardX(i), finalY, cardTypes[i], i == selected);

    const char* guide = "<- -> 선택   Space 확정";
    int guideLen = (int)strlen(guide);
    int guideX = (CONSOLE_WIDTH - guideLen) / 2;
    int guideY = finalY + CARD_HEIGHT + 2;

    SetColor(Color::LIGHT_GRAY);
    GotoXY(guideX, guideY);
    cout << guide;
    SetColor();

    while (!confirmed)
    {
        UpdateInput();

        if (GetKeyDown(VK_LEFT) && selected > 0)
        {
            DrawCard(CardX(selected), finalY, cardTypes[selected], false);
            --selected;
            DrawCard(CardX(selected), finalY, cardTypes[selected], true);
        }
        else if (GetKeyDown(VK_RIGHT) && selected < CARD_COUNT - 1)
        {
            DrawCard(CardX(selected), finalY, cardTypes[selected], false);
            ++selected;
            DrawCard(CardX(selected), finalY, cardTypes[selected], true);
        }
        else if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
        {
            confirmed = true;
        }

        FrameSync(FPS);
    }

    // ── 정리: 카드/문구 전부 지우고 스킬 추가 ──────────────
    for (int i = 0; i < CARD_COUNT; ++i)
        EraseCard(CardX(i), finalY);

    EraseLine(guideX, guideY, guideLen);
    EraseLine(titleX, titleY, titleLen);

    AddSkill(cardTypes[selected]);
}