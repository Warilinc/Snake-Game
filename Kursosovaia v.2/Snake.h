#pragma once
#include "ConsoleScreen.h"
#include "SCoord.h"
#include <vector>

typedef std::vector<SCoord> CoordVector;
/// <summary>
/// ����� ������
/// </summary>
class CSnake {
public:
    void reset(SCoord start_pos); //�������� ������ �� ���������� ���������
    void print(ConsoleScreen& scr); //���������� ������ � �������
    void move(const SCoord& delta, ConsoleScreen& scr); //�������� ������ �� ���������� delta
    void grow(const SCoord& pos, int grow); //����������� ������ �� grow
    bool into(const SCoord& pos); //������� ����������� ��������� �� ������ � ���� ������
    SCoord head();  //������� ������������ ���������� ������ ������
    int size(); //������� ������������ ������ ������
    WORD Color = 15; //���� ������
private:
    CoordVector tail; //������ ��������� ������
    unsigned int printed=0; //����������� ������������ ���������
};

