#include "SCoord.h"
/// <summary>
/// оператор присвоения со сложением для структуры SCoord
/// </summary>
/// <param name="pos">координаты с которыми складывать</param>
/// <returns>сумма которорая присвоена</returns>
SCoord& SCoord::operator +=(const SCoord& pos) {
    x += pos.x;
    y += pos.y;
    return *this;
}

/// <summary>
/// оператор сложенеия для структуры SCoord
/// </summary>
/// <param name="pos1">первая координата</param>
/// <param name="pos2">вторая координата</param>
/// <returns>сумма координат</returns>
SCoord operator +(const SCoord& pos1, const SCoord& pos2) {
    return SCoord(pos1.x + pos2.x, pos1.y + pos2.y);
}

/// <summary>
/// оператор сравнения для структуры SCoord
/// </summary>
/// <param name="pos1">первая координата</param>
/// <param name="pos2">вторая координата</param>
/// <returns>равны координаты или нет</returns>
bool operator ==(const SCoord& pos1, const SCoord& pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y;
}
