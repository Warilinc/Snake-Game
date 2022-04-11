#include <iostream>
#include "ConsoleScreen.h"
#include "SCoord.h"
#include "Game.h"


int main()
{
    setlocale(LC_ALL, "Russian"); //установить локальный язык русский
    ConsoleScreen screen; //инициализация отриосовщика консоли
    screen.cursor_visibility(false); //сделать курсор невидимым
    screen.cls(); //очистить консоль
    CGame game(screen, 80, 24, 120); //инициализация игры с начальными настройками
    game.Logo(); //вывод лого игры
    do {
        game.game_loop(); //цикл игры
    } while (game.once_more()); //запуск цикла игры до тех пор пока игрок отвечает что хочет играть
    game.bye(); //вывод прощания
   return 0;
}

