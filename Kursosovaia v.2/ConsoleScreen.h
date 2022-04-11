#pragma once
#include <windows.h>
/// <summary>
/// ����� ������ �������
/// </summary>
class ConsoleScreen {
private:
    WORD CurrentColor = 7; //������� ���� ������
    HANDLE hConsoleOut;    //���������� �������
    CONSOLE_CURSOR_INFO oldCurInfo, newCurInfo; //������ � ����� ���������� � ������� � �������

public:
    ConsoleScreen(); //�����������
    ~ConsoleScreen(); //����������
    void cursor_visibility(bool state); //��������� �������
    void pos(int x, int y, char ch = 0, WORD col = 7); //����������������� �������, ����� ������� ch ����� col
    void pos_str(int x, int y, const char* str, WORD col = 7);//���������������� �������, ����� ������ str ����� col
    void cls(); //�������� �����
    void setColor(WORD col); //���������� ���� ������ ��� �������
};