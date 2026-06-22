#include "Game.h"
#include "MainScreen.h"

#include "Bow.h"
#include "Arrow.h"
#include "Target.h"
#include "Obstacle.h"
#include "Skill.h"
#include "Card.h"

static void RunGame()
{
    Init();   // 스테이지 1 시작, 스킬 슬롯 초기화 포함

    const float deltaSeconds = 1.0f / FPS;

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE))
            break;

        HandleInput();
        UpdateArrows();
        UpdateTargets();
        UpdateObstacles();
        UpdateSkills(deltaSeconds);

        // 스킬 사용 입력: 1, 2, 3, 4 키 -> 0-based 슬롯
        if (GetKeyDown('1')) TryUseSkill(0);
        if (GetKeyDown('2')) TryUseSkill(1);
        if (GetKeyDown('3')) TryUseSkill(2);
        if (GetKeyDown('4')) TryUseSkill(3);

        // 스테이지 클리어 판정
        if (AllTargetsCleared())
        {
            ShowStageClear(currentStage);

            int nextStage = currentStage + 1;
            if (nextStage > MAX_STAGE)
            {
                ShowGameClear();
                break;
            }

            // 카드 3장 중 하나를 골라 스킬 획득
            RunCardSelection();

            StartStage(nextStage);
        }

        DrawStageHUD();   // 남은 과녁 수 실시간 갱신
        DrawSkillBox();   // 스킬 박스 UI, 쿨타임 실시간 갱신

        FrameSync(FPS);
    }
}

int main()
{
    while (true)
    {
        int selectedMenu = ShowMainScreen();

        if (selectedMenu == 1)
        {
            RunGame();
            break;
        }

        if (selectedMenu == 2)
        {
            OpenWindowsSoundSettings();
            continue;
        }

        if (selectedMenu == 3)
            break;
    }

    system("cls");
    SetCursorVisible(true);
    GotoXY(0, 0);
    cout << "Game Over!" << endl;

    return 0;
}