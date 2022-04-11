#pragma once
/// <summary>
/// ��������� ���������
/// </summary>
struct SCoord {
    int x, y; //���������� �� x � �� y
    SCoord() : x(0), y(0) {} //����������� �� ���������
    /// <summary>
    /// �����������
    /// </summary>
    /// <param name="_x">���������� �� x</param>
    /// <param name="_y">���������� �� y</param>
    SCoord(int _x, int _y) : x(_x), y(_y) {} 
    SCoord& operator +=(const SCoord& pos); // �������� ������������ �� ���������
};


SCoord operator +(const SCoord& pos1, const SCoord& pos2);//�������� �������� ���������
bool operator ==(const SCoord& pos1, const SCoord& pos2);//�������� ��������� ���������
