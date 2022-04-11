#pragma once
#include <windows.h>
/// <summary>
/// Класс экрана консоли
/// </summary>
class ConsoleScreen {
private:
    WORD CurrentColor = 7; //текущий цвет печати
    HANDLE hConsoleOut;    //дескриптор консоли
    CONSOLE_CURSOR_INFO oldCurInfo, newCurInfo; //старая и новая информация о курсоре в консоли

public:
    ConsoleScreen(); //конструктор
    ~ConsoleScreen(); //деструктор
    void cursor_visibility(bool state); //видимость курсора
    void pos(int x, int y, char ch = 0, WORD col = 7); //позиционнирование курсора, вывод символа ch цвета col
    void pos_str(int x, int y, const char* str, WORD col = 7);//позиционирование курсора, вывод строки str цвета col
    void cls(); //очистить экран
    void setColor(WORD col); //установить цвет печати для консоли
};