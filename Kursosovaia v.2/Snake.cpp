#include "Snake.h"

const char SNAKE_TAIL = '@'; //символ отрисовки хвоста змейки
const char SNAKE_HEAD = '%'; //символ отрисовки головы змейки

/// <summary>
/// сброс змейки до начального состояния
/// </summary>
/// <param name="start_pos">стартовая позиция</param>
/// <param name="tailDelta">смещение хвоста</param>
void CSnake::reset(SCoord start_pos, SCoord tailDelta) {
    tail.clear();               // очистить массив от старых значений
    tail.reserve(1000);         // зарезервировть память
    tail.push_back(start_pos);  // добавить координаты головы
    tail.push_back(start_pos);  // добавить координаты хвоста
    tail[0]+=tailDelta;         // координата хвоста смещена на дельту
}

/// <summary>
/// напечатать змейку
/// </summary>
/// <param name="scr">экран консоли</param>
void CSnake::print(ConsoleScreen& scr) {
    unsigned int tsize = tail.size() - 1;
    for (unsigned int i = 0; i < tsize; i++)
        scr.pos(tail[i].x, tail[i].y, SNAKE_TAIL, Color);
    scr.pos(tail[tsize].x, tail[tsize].y, SNAKE_HEAD, Color);
    printed = tail.size();
}

/// <summary>
/// переместить змейку
/// </summary>
/// <param name="delta">вектор смещения</param>
/// <param name="scr">экран консоли</param>
void CSnake::move(const SCoord& delta, ConsoleScreen& scr) {
    //затирание хвоста
    if (printed == tail.size())
        scr.pos(tail[0].x, tail[0].y, ' ');
    else
        printed++;

    // сдвиг координат в векторе без отрисовки
    for (unsigned int i = 1; i < tail.size(); i++)
        tail[i - 1] = tail[i];

    tail[tail.size() - 1] += delta;       // координата головы изменяется на приращение

    // Перерисовка головы и первого сегмента змеи.
    scr.pos(tail[tail.size() - 1].x, tail[tail.size() - 1].y, SNAKE_HEAD, Color);
    scr.pos(tail[tail.size() - 2].x, tail[tail.size() - 2].y, SNAKE_TAIL, Color);
}

/// <summary>
/// увеличить змейку
/// </summary>
/// <param name="pos">координата вставки</param>
/// <param name="grow">колличество добавляемых сегментов</param>
void CSnake::grow(const SCoord& pos, int grow) {
    for (int i = 0; i < grow; ++i)
        tail.insert(tail.begin(), pos);
}
/// <summary>
/// проверить находится ли обьект в змейке
/// </summary>
/// <param name="pos">позиция обьекта</param>
/// <returns>находится или ненаходится обьект в змейке</returns>
bool CSnake::into(const SCoord& pos) {
    for (unsigned int i = 0; i < tail.size(); i++)
        if (tail[i].x == pos.x && tail[i].y == pos.y)
            return true;
    return false;
}
/// <summary>
/// координаты головы змейки
/// </summary>
/// <returns>координаты головы змейки</returns>
SCoord CSnake::head() {
    return tail[tail.size() - 1];
}

/// <summary>
/// размер змейки
/// </summary>
/// <returns>размер змейки</returns>
int CSnake::size() {
    return tail.size();
}
