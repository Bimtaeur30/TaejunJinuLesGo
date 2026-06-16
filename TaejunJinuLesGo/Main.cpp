#include "Console.h"
#include "Game.h"
#include "MainScreen.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <Windows.h>

int main()
{
    srand((unsigned)time(nullptr));

    while (true)
    {
        int choice = ShowMainScreen();

        system("cls");

        switch (choice)
        {
        case 1:
        {
            SetColor(Color::LIGHT_GREEN);
            GotoXY(30, 13);
            std::cout << "게임을 시작하겠습니다!";
            SetColor();

            Sleep(800);

            RunGame();

            break;
        }

        case 2:
        {
            SetColor(Color::LIGHT_BLUE);
            GotoXY(21, 13);
            std::cout << "레전드 대기업의 소리설정 창키는중....";
            SetColor();

            if (!OpenWindowsSoundSettings())
            {
                SetColor(Color::LIGHT_RED);
                GotoXY(25, 15);
                std::cout << "소리 설정 창을 열지 못했습니다.";
                SetColor();
            }

            Sleep(1000);

            break;
        }

        case 3:
        {
            SetColor(Color::LIGHT_RED);
            GotoXY(31, 13);
            std::cout << "게임을 종료하겠습니다";
            SetColor();

            Sleep(1000);

            system("cls");
            SetCursorVisible(true);
            return 0;
        }
        }
    }
}