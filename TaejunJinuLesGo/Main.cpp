#include "Game.h"
#include "MainScreen.h"

#include "Bow.h"
#include "Arrow.h"
#include "Target.h"
#include "Obstacle.h"
#include "Skill.h"
#include "Card.h"

#include <cstdlib>
#include <string>

namespace
{
    enum class GameResult
    {
        GameOver,
        GameClear,
        ExitGame
    };

    enum class GameOverAction
    {
        Retry,
        MainMenu,
        ExitProgram
    };

    bool HasActiveArrow()
    {
        for (int i = 0; i < MAX_ARROWS; ++i)
        {
            if (arrows[i].active)
                return true;
        }

        return false;
    }

    int GetRemainingTargetCount()
    {
        int remaining = 0;

        for (int i = 0; i < targetCount; ++i)
        {
            if (targets[i].active)
                ++remaining;
        }

        return remaining;
    }

    bool HasTargetWaitingForHit()
    {
        for (int i = 0; i < targetCount; ++i)
        {
            // removing == true means the arrow already hit this target,
            // but the blink animation has not finished yet.
            // Do not count that target as a game-over reason.
            if (targets[i].active && !targets[i].removing)
                return true;
        }

        return false;
    }

    bool IsGameOver()
    {
        return ammo <= 0 && !HasActiveArrow() && HasTargetWaitingForHit();
    }

    void DrawText(int x, int y, Color color, const string& text)
    {
        GotoXY(x, y);
        SetColor(color);
        cout << text;
        SetColor();
    }

    void DrawCenteredText(int y, Color color, const string& text)
    {
        int x = (CONSOLE_WIDTH - (int)text.length()) / 2;
        if (x < 1) x = 1;

        DrawText(x, y, color, text);
    }

    void DrawBox(int x, int y, int width, int height, Color color)
    {
        SetColor(color);

        GotoXY(x, y);
        cout << '+';
        for (int i = 0; i < width - 2; ++i) cout << '-';
        cout << '+';

        for (int row = 1; row < height - 1; ++row)
        {
            GotoXY(x, y + row);
            cout << '|';

            GotoXY(x + width - 1, y + row);
            cout << '|';
        }

        GotoXY(x, y + height - 1);
        cout << '+';
        for (int i = 0; i < width - 2; ++i) cout << '-';
        cout << '+';

        SetColor();
    }

    void DrawGameOverTitle()
    {
        DrawCenteredText(5, Color::LIGHT_RED, "  _____                         ____                 ");
        DrawCenteredText(6, Color::LIGHT_RED, " / ____|                       / __ \\                ");
        DrawCenteredText(7, Color::LIGHT_RED, "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ ");
        DrawCenteredText(8, Color::LIGHT_RED, "| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|");
        DrawCenteredText(9, Color::LIGHT_RED, "| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   ");
        DrawCenteredText(10, Color::LIGHT_RED, " \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   ");
    }

    GameOverAction ShowGameOverScreen()
    {
        system("cls");
        SetConsoleWindowSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
        SetConsoleWindowStyle(true);
        SetCursorVisible(false);
        SetConsoleMouseInputDisabled();

        DrawBorder();
        DrawBox(10, 3, 60, 23, Color::LIGHT_RED);
        DrawGameOverTitle();

        DrawCenteredText(13, Color::WHITE, "All arrows have been used.");
        DrawCenteredText(14, Color::LIGHT_GRAY, "You ran out of ammo before clearing every target.");

        DrawText(25, 17, Color::LIGHT_YELLOW, "Reached Stage : ");
        SetColor(Color::WHITE);
        cout << currentStage << "/" << MAX_STAGE;
        SetColor();

        DrawText(25, 18, Color::LIGHT_YELLOW, "Targets Left  : ");
        SetColor(Color::WHITE);
        cout << GetRemainingTargetCount() << "/" << targetCount;
        SetColor();

        DrawText(25, 19, Color::LIGHT_YELLOW, "Ammo Left     : ");
        SetColor(Color::WHITE);
        cout << ammo;
        SetColor();

        DrawCenteredText(22, Color::CYAN, "[ R ] Retry    [ M ] Main Menu    [ ESC ] Exit");

        while (true)
        {
            if (_kbhit())
            {
                int key = _getch();

                if (key == 'r' || key == 'R')
                    return GameOverAction::Retry;

                if (key == 'm' || key == 'M')
                    return GameOverAction::MainMenu;

                if (key == VK_ESCAPE || key == 27)
                    return GameOverAction::ExitProgram;
            }

            Sleep(50);
        }
    }

    void ResetConsoleForExit()
    {
        system("cls");
        SetCursorVisible(true);
        GotoXY(0, 0);
    }
}

static GameResult RunGame()
{
    Init();

    const float deltaSeconds = 1.0f / FPS;

    while (true)
    {
        UpdateInput();

        if (GetKeyDown(VK_ESCAPE))
            return GameResult::ExitGame;

        HandleInput();
        UpdateArrows();
        UpdateTargets();
        UpdateObstacles();
        UpdateSkills(deltaSeconds);

        if (GetKeyDown('1')) TryUseSkill(0);
        if (GetKeyDown('2')) TryUseSkill(1);
        if (GetKeyDown('3')) TryUseSkill(2);
        if (GetKeyDown('4')) TryUseSkill(3);

        if (AllTargetsCleared())
        {
            ShowStageClear(currentStage);

            int nextStage = currentStage + 1;
            if (nextStage > MAX_STAGE)
            {
                Sleep(1200);
                return GameResult::GameClear;
            }

            RunCardSelection();
            StartStage(nextStage);
        }

        if (IsGameOver())
            return GameResult::GameOver;

        DrawStageHUD();
        DrawSkillBox();

        FrameSync(FPS);
    }
}

int main()
{
    bool isProgramRunning = true;

    while (isProgramRunning)
    {
        int selectedMenu = ShowMainScreen();

        if (selectedMenu == 1)
        {
            while (true)
            {
                GameResult result = RunGame();

                if (result == GameResult::GameOver)
                {
                    GameOverAction action = ShowGameOverScreen();

                    if (action == GameOverAction::Retry)
                        continue;

                    if (action == GameOverAction::MainMenu)
                        break;

                    if (action == GameOverAction::ExitProgram)
                    {
                        isProgramRunning = false;
                        break;
                    }
                }
                else if (result == GameResult::GameClear)
                {
                    break;
                }
                else if (result == GameResult::ExitGame)
                {
                    isProgramRunning = false;
                    break;
                }
            }
        }
        else if (selectedMenu == 2)
        {
            OpenWindowsSoundSettings();
        }
        else if (selectedMenu == 3)
        {
            isProgramRunning = false;
        }
    }

    ResetConsoleForExit();
    return 0;
}