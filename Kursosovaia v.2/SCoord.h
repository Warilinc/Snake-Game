#pragma once
/// <summary>
/// структура координат
/// </summary>
struct SCoord {
    int x, y; //координата по x и по y
    SCoord() : x(0), y(0) {} //конструктор по умолчанию
    /// <summary>
    /// конструктор
    /// </summary>
    /// <param name="_x">координата по x</param>
    /// <param name="_y">координата по y</param>
    SCoord(int _x, int _y) : x(_x), y(_y) {} 
    SCoord& operator +=(const SCoord& pos); // оператор присваивания со сложением
};


SCoord operator +(const SCoord& pos1, const SCoord& pos2);//оператор сложения координат
bool operator ==(const SCoord& pos1, const SCoord& pos2);//оператор сравнения координат
