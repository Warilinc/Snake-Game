#include "ConsoleScreen.h"
#include <conio.h>
/// <summary>
/// �����������
/// </summary>
ConsoleScreen::ConsoleScreen() {
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE); //��������� ����������� �������
    GetConsoleCursorInfo(hConsoleOut, &oldCurInfo); //��������� ���������� � �������
    newCurInfo.dwSize = oldCurInfo.dwSize;
    newCurInfo.bVisible = oldCurInfo.bVisible; 
}
/// <summary>
/// ����������
/// </summary>
ConsoleScreen::~ConsoleScreen() {
    SetConsoleCursorInfo(hConsoleOut, &oldCurInfo);
}
/// <summary>
/// ��������� ��������� �������
/// </summary>
/// <param name="state">�������� ��/���</param>
void ConsoleScreen::cursor_visibility(bool state) {
    newCurInfo.bVisible = state;
    SetConsoleCursorInfo(hConsoleOut, &newCurInfo);
}
/// <summary>
/// ����� �������� ����� ������
/// </summary>
/// <param name="col">����</param>
void ConsoleScreen::setColor(WORD col) {
    CurrentColor = col;
    SetConsoleTextAttribute(hConsoleOut, col); 
}
/// <summary>
/// ���������������� �������, ����� ������ ������ ������� ch ���� ������������.
/// </summary>
/// <param name="x">���������� x</param>
/// <param name="y">���������� y</param>
/// <param name="ch">������ ��� ������</param>
/// <param name="col">����</param>
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
/// ������������������ ����� ������ ������� �����
/// </summary>
/// <param name="x">���������� x</param>
/// <param name="y">���������� y</param>
/// <param name="str">������ ��� ������</param>
/// <param name="col">����</param>
void ConsoleScreen::pos_str(int x, int y, const char* str, WORD col) {
    pos(x, y);
    if (col != CurrentColor) setColor(col);
    _cprintf("%s", str);
}
/// <summary>
/// �������� �������
/// </summary>
void ConsoleScreen::cls() {
    system("cls");
}
