#include <iostream>
#include "towers.h"
#include <windows.h>
#include <vector>
using namespace std;
using namespace TowerNS;

ChrTw T1(2, 1, 2, 2), T2(3, 2, 3, 3);
ChrTw TablT[] = { T1,T2 };
ChrCst C1(1, 20, 1, 1), C2(2, 50, 2, 2);
ChrCst TablC[] = { C1,C2 };
Meat Den("Den", 0, 5, 2, 1, 1); Heavy FE("FE", 0, 10, 1, 1, 1); Plane HG("HG", 0, 4, 1, 1, 1);
Enemy* TablL[] = { &Den,&FE,&HG };
int TimeRl[] = { -1,6, 10 };

int main()
{
    Coord D;
    D.x = 40; D.y = 30;
    Level Game(D);
    Castle Kam(TablC);
    D.x = 5; D.y = 5;
    Game.SetBLG(D, &Kam, 6);
    D.x = 30; D.y = 20;
    Lair Evl(TablL, TimeRl);
    Game.SetBLG(D, &Evl, 5);
    std::cout << Game << std::endl;
    Tower T1(TablT);
    Tower T2(TablT);
    T2.LVLUP();
    int n = 0;
    std::cout << "How many do you want add Tower 1 LVL?" << std::endl;
    std::cin >> n;
    for (int l = 0; l < n; l++) {
        do {
            std::cout << "Enter coordinate for Tower 1 LVL" << std::endl;
            std::cin >> D.x >> D.y;
        } while (Game.GetType(D) != 0);
        Game.SetBLG(D, &T1, 4);
        system("cls");
        std::cout << Game << std::endl;
    }

    do {
        std::cout << "Enter coordinate for Tower 2 LVL" << std::endl;
        std::cin >> D.x >> D.y;
    } while (Game.GetType(D) != 0);
    Game.SetBLG(D, &T2, 4);
    system("cls");
    std::cout << Game << std::endl;
    int ch = 0;
    while (ch == 0)
    {
        Game.Relize(&Evl);
        ch = Game.Step(TablL, &T1, &T2);
        Sleep(300);
        system("cls");
        std::cout << Game << std::endl;
    }
    if (ch == -1)
        std::cout << "YOU WON" << std::endl;
    else
        std::cout << "YOU LOSE" << std::endl;
    return 0;
}