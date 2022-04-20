#pragma once
#include "ConsoleScreen.h"
#include "SCoord.h"
#include "Snake.h"
/// <summary>
/// Класс игры
/// </summary>
class CGame {
public:
    CGame(ConsoleScreen& _scr, int _width = 80, int _height = 24, int _latency = 100); //конструктор
    void game_loop();           // основной цикл игры
    bool once_more();           // вывод запроса и приём ответа от игрока
    void pak(int y);            // "Press any key for continue"
    void Settings();            //настройки игры
    void print_stat();          // напечатать статистику
    void Logo();                //вывести лого игры
    void bye();                 //вывести прощание


private:
    enum class Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN, CMD_PLAY}; //команды игры
    enum class State { ST_OK, ST_EXIT, ST_DIED };  //состояния игры

    typedef std::pair<int, Command> CmdPair;

    int width, height;          //размеры поля
    int latency;                //задержка между действиями
    int SegmentsGet=3;          //колличество получаемых змейкой сегментов
    int FoodOnMap=1;            //колличество еды на поле
    bool borderless = false;    //режим без границ включен/выключен

    ConsoleScreen scr;                  //консоль
    CSnake snake;                       //змейка
    CmdPair cmd_table[6];               //таблица комманд
    double duration_game;               //длительность игры
    double rating, rating_max=0.0;      //рейтинг и максимальный рейтинг
    WORD bdtxtColor = 7;                //цвет границ игрового поля и текста
    WORD foodColor = 15;                //цвет еды
    void draw_field();                  //нарисовать игровое поле
    SCoord make_food();                 //создать еду
    SCoord foodVect[5]{SCoord()};       //вектор координат еды
    Command get_command();              //получить и обработать комманду
};

