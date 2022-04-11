#include "SCoord.h"
/// <summary>
/// �������� ���������� �� ��������� ��� ��������� SCoord
/// </summary>
/// <param name="pos">���������� � �������� ����������</param>
/// <returns>����� ��������� ���������</returns>
SCoord& SCoord::operator +=(const SCoord& pos) {
    x += pos.x;
    y += pos.y;
    return *this;
}

/// <summary>
/// �������� ��������� ��� ��������� SCoord
/// </summary>
/// <param name="pos1">������ ����������</param>
/// <param name="pos2">������ ����������</param>
/// <returns>����� ���������</returns>
SCoord operator +(const SCoord& pos1, const SCoord& pos2) {
    return SCoord(pos1.x + pos2.x, pos1.y + pos2.y);
}

/// <summary>
/// �������� ��������� ��� ��������� SCoord
/// </summary>
/// <param name="pos1">������ ����������</param>
/// <param name="pos2">������ ����������</param>
/// <returns>����� ���������� ��� ���</returns>
bool operator ==(const SCoord& pos1, const SCoord& pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y;
}
