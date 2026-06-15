#include "Game.h"
#include "Bow.h"
#include "Arrow.h"
#include "Target.h"

int main()
{
    Init();

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE)) break;

        HandleInput();
        UpdateArrows();
        UpdateTarget();

        FrameSync(FPS);
    }

    system("cls");
    SetCursorVisible(true);
    GotoXY(0, 0);
    cout << "Game Over!" << endl;

    return 0;
}