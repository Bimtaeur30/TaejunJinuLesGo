#include "Game.h"
#include "Bow.h"
#include "Arrow.h"
#include "Target.h"

int main()
{
    Init();   // 스테이지 1 시작

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE)) break;

        HandleInput();
        UpdateArrows();
        UpdateTargets();

        // ── 스테이지 클리어 판정 ──────────────────────────
        if (AllTargetsCleared())
        {
            ShowStageClear(currentStage);

            int nextStage = currentStage + 1;
            if (nextStage > MAX_STAGE)
            {
                ShowGameClear();
                break;
            }

            StartStage(nextStage);
        }
        // ─────────────────────────────────────────────────

        DrawStageHUD();   // 남은 과녁 수 실시간 갱신

        FrameSync(FPS);
    }

    system("cls");
    SetCursorVisible(true);
    GotoXY(0, 0);
    cout << "Game Over!" << endl;

    return 0;
}