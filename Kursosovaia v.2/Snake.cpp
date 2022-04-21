#include "Snake.h"

const char SNAKE_TAIL = '@'; //������ ��������� ������ ������
const char SNAKE_HEAD = '%'; //������ ��������� ������ ������

/// <summary>
/// ����� ������ �� ���������� ���������
/// </summary>
/// <param name="start_pos">��������� �������</param>
/// <param name="tailDelta">�������� ������</param>
void CSnake::reset(SCoord start_pos, SCoord tailDelta) {
    tail.clear();               // �������� ������ �� ������ ��������
    tail.reserve(1000);         // �������������� ������
    tail.push_back(start_pos);  // �������� ���������� ������
    tail.push_back(start_pos);  // �������� ���������� ������
    tail[0]+=tailDelta;         // ���������� ������ ������� �� ������
}

/// <summary>
/// ���������� ������
/// </summary>
/// <param name="scr">����� �������</param>
void CSnake::print(ConsoleScreen& scr) {
    unsigned int tsize = tail.size() - 1;
    for (unsigned int i = 0; i < tsize; i++)
        scr.pos(tail[i].x, tail[i].y, SNAKE_TAIL, Color);
    scr.pos(tail[tsize].x, tail[tsize].y, SNAKE_HEAD, Color);
    printed = tail.size();
}

/// <summary>
/// ����������� ������
/// </summary>
/// <param name="delta">������ ��������</param>
/// <param name="scr">����� �������</param>
void CSnake::move(const SCoord& delta, ConsoleScreen& scr) {
    //��������� ������
    if (printed == tail.size())
        scr.pos(tail[0].x, tail[0].y, ' ');
    else
        printed++;

    // ����� ��������� � ������� ��� ���������
    for (unsigned int i = 1; i < tail.size(); i++)
        tail[i - 1] = tail[i];

    tail[tail.size() - 1] += delta;       // ���������� ������ ���������� �� ����������

    // ����������� ������ � ������� �������� ����.
    scr.pos(tail[tail.size() - 1].x, tail[tail.size() - 1].y, SNAKE_HEAD, Color);
    scr.pos(tail[tail.size() - 2].x, tail[tail.size() - 2].y, SNAKE_TAIL, Color);
}

/// <summary>
/// ��������� ������
/// </summary>
/// <param name="pos">���������� �������</param>
/// <param name="grow">����������� ����������� ���������</param>
void CSnake::grow(const SCoord& pos, int grow) {
    for (int i = 0; i < grow; ++i)
        tail.insert(tail.begin(), pos);
}
/// <summary>
/// ��������� ��������� �� ������ � ������
/// </summary>
/// <param name="pos">������� �������</param>
/// <returns>��������� ��� ����������� ������ � ������</returns>
bool CSnake::into(const SCoord& pos) {
    for (unsigned int i = 0; i < tail.size(); i++)
        if (tail[i].x == pos.x && tail[i].y == pos.y)
            return true;
    return false;
}
/// <summary>
/// ���������� ������ ������
/// </summary>
/// <returns>���������� ������ ������</returns>
SCoord CSnake::head() {
    return tail[tail.size() - 1];
}

/// <summary>
/// ������ ������
/// </summary>
/// <returns>������ ������</returns>
int CSnake::size() {
    return tail.size();
}
