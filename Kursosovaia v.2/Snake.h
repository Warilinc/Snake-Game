#pragma once
#include "ConsoleScreen.h"
#include "SCoord.h"
#include <vector>

typedef std::vector<SCoord> CoordVector;
/// <summary>
/// Класс змейки
/// </summary>
class CSnake {
public:
    void reset(SCoord start_pos); //сбросить змейку до начального состояния
    void print(ConsoleScreen& scr); //напечатать змейку в консоли
    void move(const SCoord& delta, ConsoleScreen& scr); //сдвинуть змейку на координаты delta
    void grow(const SCoord& pos, int grow); //увериличить змейку на grow
    bool into(const SCoord& pos); //функция проверяющая находится ли обьект в теле змейки
    SCoord head();  //функция возвращающая координаты головы змейки
    int size(); //функция возвращающая размер змейки
    WORD Color = 15; //цвет змейки
private:
    CoordVector tail; //вектор координат змейки
    unsigned int printed=0; //колличество напечатанных сегментов
};

