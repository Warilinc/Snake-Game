#include "ConsoleScreen.h"
#include <conio.h>
/// <summary>
/// конструктор
/// </summary>
ConsoleScreen::ConsoleScreen() {
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE); //получение дескриптора консоли
    GetConsoleCursorInfo(hConsoleOut, &oldCurInfo); //получение информации о курсоре
    newCurInfo.dwSize = oldCurInfo.dwSize;
    newCurInfo.bVisible = oldCurInfo.bVisible; 
}
/// <summary>
/// деструктор
/// </summary>
ConsoleScreen::~ConsoleScreen() {
    SetConsoleCursorInfo(hConsoleOut, &oldCurInfo);
}
/// <summary>
/// установка видимости курсора
/// </summary>
/// <param name="state">Параметр да/нет</param>
void ConsoleScreen::cursor_visibility(bool state) {
    newCurInfo.bVisible = state;
    SetConsoleCursorInfo(hConsoleOut, &newCurInfo);
}
/// <summary>
/// Выбор текущего цвета вывода
/// </summary>
/// <param name="col">цвет</param>
void ConsoleScreen::setColor(WORD col) {
    CurrentColor = col;
    SetConsoleTextAttribute(hConsoleOut, col); 
}
/// <summary>
/// позиционирование курсора, вывод нужным цветом символа ch если присутствует.
/// </summary>
/// <param name="x">координата x</param>
/// <param name="y">координата y</param>
/// <param name="ch">символ для вывода</param>
/// <param name="col">цвет</param>
void ConsoleScreen::pos(int x, int y, char ch, WORD col) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsoleOut, pos);
    if (ch > 0) {    
        if (col != CurrentColor) setColor(col);
        _putch(ch);
    }
        
}
/// <summary>
/// Позиционнированный вывод строки нужного цвета
/// </summary>
/// <param name="x">координата x</param>
/// <param name="y">координата y</param>
/// <param name="str">строка для вывода</param>
/// <param name="col">цвет</param>
void ConsoleScreen::pos_str(int x, int y, const char* str, WORD col) {
    pos(x, y);
    if (col != CurrentColor) setColor(col);
    _cprintf("%s", str);
}
/// <summary>
/// очистить консоль
/// </summary>
void ConsoleScreen::cls() {
    system("cls");
}
