#include "Game.h"
#include <conio.h>
#include <iostream>

const char FOOD = '*'; //символ отрисовки еды
const char BORDER = '#'; //символ отрисовки границы

/// <summary>
/// очистить хринилище комманд
/// </summary>
void clearkeys() {
    while (_kbhit())
        _getch();
}
/// <summary>
/// конструктор
/// </summary>
/// <param name="_scr">дескриптор консоли</param>
/// <param name="_width">ширина игрового поля</param>
/// <param name="_height">высота игрового поля</param>
/// <param name="_latency">задержка между передвижением</param>
CGame::CGame(ConsoleScreen& _scr, int _width, int _height, int _latency) :
    width(_width), height(_height), latency(_latency), scr(_scr) {

    //инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(NULL)));

    // инициализация таблицы команд управления игрой
    cmd_table[0] = CmdPair(27, Command::CMD_EXIT);   // escape key
    cmd_table[1] = CmdPair('K', Command::CMD_LEFT);  // стрелка влево
    cmd_table[2] = CmdPair('M', Command::CMD_RIGHT); // стрелка вправо
    cmd_table[3] = CmdPair('H', Command::CMD_UP);    // стрелка вверх
    cmd_table[4] = CmdPair('P', Command::CMD_DOWN);  // стрелка вниз
    cmd_table[5] = CmdPair(13, Command::CMD_PLAY);  // enter
    cmd_table[6] = CmdPair('a', Command::CMD_2LEFT);  // a
    cmd_table[7] = CmdPair('d', Command::CMD_2RIGHT); // d
    cmd_table[8] = CmdPair('w', Command::CMD_2UP);    // w
    cmd_table[9] = CmdPair('s', Command::CMD_2DOWN);  // s
}

/// <summary>
/// получить и обработать комманду
/// </summary>
/// <returns>полученная комманда</returns>
CGame::Command CGame::get_command() {
    int ch;
    //комментарий
    ch = _getch();
    if (ch == 0 || ch == 0xe0) {
        ch = _getch();
    }

    for (int i = 0; i < 10; i++) {
        if (cmd_table[i].first == ch || cmd_table[i].first == tolower(ch)) {
            return cmd_table[i].second;
        }
    }
    return Command::CMD_NOCOMMAND;
}

/// <summary>
/// создать еду
/// </summary>
/// <returns>координаты для расположения еды</returns>
SCoord CGame::make_food() {
    SCoord food;
    if ((height-2)*(width-2) - snake.size() >= FoodOnMap || (twoplayers && (height - 2) * (width - 2) - snake.size() - snake2.size() >= FoodOnMap)) // проверка хватает ли еде места
    do {
        food.x = rand() % (width - 2) + 1;
        food.y = rand() % (height - 2) + 1;
        for (int i = 0; i < FoodOnMap; i++) if (food.x == foodVect[i].x && food.y == foodVect[i].y) continue;
    } while (snake.into(food) || (twoplayers && snake2.into(food)));

    return food;
}


/// <summary>
/// нарисовать игровое поле
/// </summary>
void CGame::draw_field() {

    scr.cls();

    for (int y = 0; y < height; y++) {
        if (y == 0 || y == height - 1) {
            for (int x = 0; x < width; x++)
                scr.pos(x, y, BORDER, bdColor);
        }
        else {
            scr.pos(0, y, BORDER, bdColor);
            scr.pos(width - 1, y, BORDER, bdColor);
        }
    }

    scr.pos_str(0, height, "Length: ****  Rating: ******.**** (******.****)  Time: ****.**", txtColor);
    if (twoplayers) {
        scr.pos_str(66, height, "Player 1", snake.Color);
        scr.pos_str(77, height, "Wins: *", txtColor);
        scr.pos_str(0, height + 1, "Length: ****  Rating: ******.**** (******.****)  Time: ****.**", txtColor);
        scr.pos_str(66, height+1, "Player 2", snake2.Color);
        scr.pos_str(77, height+1, "Wins: *", txtColor);
    }
    
}

/// <summary>
/// Press any key to continue
/// </summary>
/// <param name="y">высота</param>
void CGame::pak(int y) {
    scr.pos_str(width / 2 - 10, y, "Press any key to continue..");
    scr.pos_str(width / 2 - 10, y+2, "[S]ettings");
    int ch = _getch();
    clearkeys();
    if (ch == 's' || ch == 'S') Settings();
}

/// <summary>
/// напечатать статистику
/// </summary>
void CGame::print_stat() {
    scr.setColor(txtColor);
    scr.pos(8, height);
    _cprintf("%04u", snake.size()); //длина змейки
    scr.pos(22, height);
    _cprintf("%011.4f", rating); //рейтинг текущей игры
    scr.pos(35, height);
    _cprintf("%011.4f", rating_max); //максимальный рейтинг
    scr.pos(55, height);
    _cprintf("%07.2f", duration_game); //время игры
    if (twoplayers) {
        scr.pos(8, height + 1);
        _cprintf("%04u", snake2.size()); //длина змейки
        scr.pos(22, height + 1);
        _cprintf("%011.4f", rating2); //рейтинг текущей игры
        scr.pos(35, height + 1);
        _cprintf("%011.4f", rating_max2); //максимальный рейтинг
        scr.pos(55, height + 1);
        _cprintf("%07.2f", duration_game2); //время игры

        //победы игроков
        scr.pos(83, height);
        _cprintf("%u", winsPlayer1);
        scr.pos(83, height+1);
        _cprintf("%u", winsPlayer2);
    }
}

/// <summary>
/// спросить игрока хочет ли он сыграть еще раз
/// </summary>
/// <returns>да/нет</returns>
bool CGame::once_more() {
    scr.pos_str(width / 2 - 16, height - 3, "Are you want to play once more?", txtColor);
    scr.pos_str(width / 2 - 6, height - 2, "[S]ettings", txtColor);
    
    int ch = _getch();
    clearkeys();
    if (ch == 's' || ch == 'S') Settings();
    if (ch == 'N' || ch == 'n' || ch == 27)
        return false;
    return true;
}

/// <summary>
/// напечатать лого
/// </summary>
void CGame::Logo() {
    scr.pos_str(width / 2 - 14, height / 2-2, " ####   ##  ##   ####   ##  ##  #####", 15);
    scr.pos_str(width / 2 - 14, height / 2-1, "##      ### ##  ##  ##  ## ##   ##   ", 15);
    scr.pos_str(width / 2 - 14, height / 2,   " ####   ## ###  ######  ####    #### ", 15);
    scr.pos_str(width / 2 - 14, height / 2+1, "    ##  ##  ##  ##  ##  ## ##   ##   ", 15);
    scr.pos_str(width / 2 - 14, height / 2+2, " ####   ##  ##  ##  ##  ##  ##  #####", 15);
    _getch();
    clearkeys();
    pak(height / 2 + 4);
}

/// <summary>
/// настройки игры
/// </summary>
void CGame::Settings() {
    scr.cls();
    int setwidth = width > 120 ? width : 120;
    int setheight = height > 30 ? height : 30;
    scr.pos_str(12, 1, "Settings", 7);
    scr.pos_str(setwidth-9-12, 1, "[Esc]ape", 7);
    scr.pos_str(setwidth / 2 - 11 , setheight - 3, "Press [Enter] to play", 7);

    Command cmd = Command::CMD_NOCOMMAND;
    int delta = 0, deltaMenu = 0;
    

    do {
        if (delta == 0) {
            scr.setColor(15);
        } else{
            scr.setColor(7);
        }
        scr.pos((setwidth - 55) / 2, 2);
    
        if (deltaMenu == 0) {
            _cprintf(" < Field >      Colors        Difficulty        Mode   ");

            scr.pos(setwidth / 2 - 9, (setheight - 1) / 2 - 1);
            scr.setColor(15);
            if (delta == 1)  
                _cprintf(" > Height: %03u < ", height);
            else
                _cprintf("   Height: %03u   ", height);

            scr.pos(setwidth / 2 - 9, (setheight - 1) / 2 + 1);
            if (delta == 2)           
                _cprintf(" > Width: %03u < ", width);
            else
                _cprintf("   Width: %03u   ", width);

        } else if (deltaMenu == 1) {
            _cprintf("   Field      < Colors >      Difficulty        Mode   ");

            

            if (delta == 1)
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 - 4," > Fruit: * < ", foodColor);
            else
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 - 4,"   Fruit: *   ", foodColor);

            if (delta == 2)
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 - 2, " > Border: ### < ", bdColor);
            else
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 - 2, "   Border: ###   ", bdColor);

            if (delta == 3)
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2, " > Text: Some Text < ", txtColor);
            else
                scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2, "   Text: Some Text   ", txtColor);

            if (!twoplayers) {
                if (delta == 4)
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 2, " > Snake: @@@% < ", snake.Color);
                else
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 2, "   Snake: @@@%   ", snake.Color);
            }
            else {
                if (delta == 4)
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 2, " > Snake 1: @@@% < ", snake.Color);
                else
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 2, "   Snake 1: @@@%   ", snake.Color);

                if (delta == 5)
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 4, " > Snake 2: @@@% < ", snake2.Color);
                else
                    scr.pos_str(setwidth / 2 - 11, (setheight - 1) / 2 + 4, "   Snake 2: @@@%   ", snake2.Color);
            }

        } else if (deltaMenu == 2) {
            _cprintf("   Field        Colors      < Difficulty >      Mode   ");

            scr.setColor(15);
            scr.pos(setwidth / 2 - 12, (setheight - 1) / 2 - 2);
            if (delta == 1)
                _cprintf(" > Segments per eat: %u < ", SegmentsGet);
            else
                _cprintf("   Segments per eat: %u   ", SegmentsGet);

            scr.pos(setwidth / 2 - 12, (setheight - 1) / 2);
            if (delta == 2)
                _cprintf(" > Food on map: %u < ", FoodOnMap);
            else
                _cprintf("   Food on map: %u   ", FoodOnMap);

            scr.pos(setwidth / 2 - 12, (setheight - 1) / 2 + 2);
            if (delta == 3)
                _cprintf(" > Speed: %s < ", latency == 120 ? "Normal" : (latency == 70 ? " Fast " : " Slow "));
            else
                _cprintf("   Speed: %s   ", latency == 120 ? "Normal" : (latency == 70 ? " Fast " : " Slow "));

        } else {
            _cprintf("   Field        Colors        Difficulty      < Mode > ");

            scr.pos(setwidth / 2 - 11, (setheight - 1) / 2 - 1);
            scr.setColor(15);
            if (delta == 1)
                _cprintf(" > Borderless: %s < ", (borderless ? "On" : "Off"));
            else
                _cprintf("   Borderless: %s   ", (borderless ? "On" : "Off"));

            scr.pos(setwidth / 2 - 11, (setheight - 1) / 2 + 1);
            if (delta == 2)
                _cprintf(" > Two players: %s < ", (twoplayers ? "On" : "Off"));
            else
                _cprintf("   Two players: %s   ", (twoplayers ? "On" : "Off"));
        }
        //печать настроек


        cmd = get_command();

        // обработка команд
        switch (cmd) {
        case Command::CMD_LEFT:
            if (delta == 0 && deltaMenu > 0) {
                deltaMenu -= 1;
                for (int i = -5; i < 5; i++) scr.pos_str(setwidth / 2 - 15, (setheight - 1) / 2 - i, "                                        ");
            }
            if (deltaMenu == 0 && delta == 1 && height>10) height -= 1;
            if (deltaMenu == 0 && delta == 2 && width> 40) width -= 1;
            
            if (deltaMenu == 1 && delta == 1 && foodColor < 15) foodColor += 1;
            if (deltaMenu == 1 && delta == 2 && bdColor < 15) bdColor += 1;
            if (deltaMenu == 1 && delta == 3 && txtColor < 15) txtColor += 1;
            if (deltaMenu == 1 && delta == 4 && snake.Color < 15) snake.Color += 1;
            if (deltaMenu == 1 && delta == 5 && snake2.Color < 15) snake2.Color += 1;

            if (deltaMenu == 2 && delta == 1 && SegmentsGet > 1) SegmentsGet -= 1;
            if (deltaMenu == 2 && delta == 2 && FoodOnMap > 1) FoodOnMap -= 1;
            if (deltaMenu == 2 && delta == 3 && latency < 170) latency += 50;

            if (deltaMenu == 3 && delta == 1) borderless = !borderless;
            if (deltaMenu == 3 && delta == 2) twoplayers = !twoplayers;
            break;
        case Command::CMD_RIGHT:
            if (delta == 0 && deltaMenu < 4) {
                deltaMenu += 1;
                for (int i = -5; i < 5; i++) scr.pos_str(setwidth / 2 - 15, (setheight - 1) / 2 - i, "                                        ");
            }
            if (deltaMenu == 0 && delta == 1 && height < 100) height += 1;
            if (deltaMenu == 0 && delta == 2 && width < 800) width += 1;

            if (deltaMenu == 1 && delta == 1 && foodColor > 1) foodColor -= 1;
            if (deltaMenu == 1 && delta == 2 && bdColor > 1) bdColor -= 1;
            if (deltaMenu == 1 && delta == 3 && txtColor > 1) txtColor -= 1;
            if (deltaMenu == 1 && delta == 4 && snake.Color > 1) snake.Color -= 1;
            if (deltaMenu == 1 && delta == 5 && snake2.Color > 1) snake2.Color -= 1;

            if (deltaMenu == 2 && delta == 1 && SegmentsGet < 5) SegmentsGet += 1;
            if (deltaMenu == 2 && delta == 2 && FoodOnMap < 5) FoodOnMap += 1;
            if (deltaMenu == 2 && delta == 3 && latency > 70) latency -= 50;
            
            if (deltaMenu == 3 && delta == 1) borderless = !borderless;
            if (deltaMenu == 3 && delta == 2) twoplayers = !twoplayers;
            break;
        case Command::CMD_UP:
            if (delta > 0)
                delta -= 1;
            else {
                if (deltaMenu == 0) delta = 2;
                if (deltaMenu == 1) delta = twoplayers ? 5 : 4 ;
                if (deltaMenu == 2) delta = 3;
                if (deltaMenu == 3) delta = 2;
            }
            break;
        case Command::CMD_DOWN:
            if (deltaMenu == 0) if (delta < 2) delta += 1; else delta = 0;
            if (deltaMenu == 1) if (delta < (twoplayers ? 5 : 4)) delta += 1; else delta = 0;
            if (deltaMenu == 2) if (delta < 3) delta += 1; else delta = 0;
            if (deltaMenu == 3) if (delta < 2) delta += 1; else delta = 0;
            
                
            break;
        default:
            break;
        };

        
    } while (cmd != Command::CMD_EXIT && cmd != Command::CMD_PLAY);
    clearkeys();
}

/// <summary>
/// прощание с игроком
/// </summary>
void CGame::bye() {
    scr.cls();
    scr.pos_str(width / 2 - 10, height / 2, "Thanks for playing", 15);
    scr.pos(0, height + 1);
}

/// <summary>
/// основной цикл игры
/// </summary>
void CGame::game_loop() { 
    scr.cursor_visibility(false); //сделать курсор невидимым
    //сбросить все на начальные значения и нарисовать поле
    draw_field();
    if (!twoplayers) {
        snake.reset(SCoord(width / 2, height / 2));
    }
    else {
        snake.reset(SCoord(width / 2 - 4, height / 2));
        snake2.reset(SCoord(width / 2 + 4, height / 2), SCoord(-1,0));
    }
    
    duration_game = 0;
    rating = 0.0;
    
    Command precmd = Command::CMD_NOCOMMAND;
    Command cmd = Command::CMD_NOCOMMAND;
    State stt = State::ST_OK;
    SCoord delta(-1, 0);
    SCoord deltaBorder;
    SCoord hd;

    duration_game2 = 0;
    rating2 = 0.0;
    Command cmd2 = Command::CMD_NOCOMMAND;
    State stt2 = State::ST_OK;
    SCoord delta2(1, 0);
    SCoord deltaBorder2;
    SCoord hd2;

    //нарисовать начальную еду и змейку
    for (int i = 0; i < FoodOnMap; i++) {
        foodVect[i] = make_food();          
        scr.pos(foodVect[i].x, foodVect[i].y, FOOD, foodColor);             
    }
    snake.print(scr);
    if (twoplayers) snake2.print(scr);

    print_stat();                       
    clock_t time1, time2, duration;
    clock_t time1s2, time2s2, durations2;
    time1 = clock();
    if (twoplayers) time1s2 = clock();

    do {
        // если в буфере клавиатуры есть информация, то принять команду
        
        while (_kbhit()) {
            precmd = get_command();
            cmd = precmd != Command::CMD_NOCOMMAND && static_cast<unsigned int>(precmd) < 6 ? precmd : cmd;
            cmd2 = static_cast<unsigned int>(precmd) > 6 ? precmd : cmd2;
        }

        // обработка команд
        switch (cmd) {
        case Command::CMD_LEFT:
            if (delta.x != 1) 
                delta = SCoord(-1, 0);
            break;
        case Command::CMD_RIGHT:
            if (delta.x != -1)
                delta = SCoord(1, 0);
            break;
        case Command::CMD_UP:
            if (delta.y != 1)
                delta = SCoord(0, -1);
            break;
        case Command::CMD_DOWN:
            if (delta.y != -1)
                delta = SCoord(0, 1);
            break;
        case Command::CMD_EXIT:
            stt = State::ST_EXIT;
        default:
            break;
        };

        switch (cmd2) {
        case Command::CMD_2LEFT:
            if (delta2.x != 1)
                delta2 = SCoord(-1, 0);
            break;
        case Command::CMD_2RIGHT:
            if (delta2.x != -1)
                delta2 = SCoord(1, 0);
            break;
        case Command::CMD_2UP:
            if (delta2.y != 1)
                delta2 = SCoord(0, -1);
            break;
        case Command::CMD_2DOWN:
            if (delta2.y != -1)
                delta2 = SCoord(0, 1);
            break;
        default:
            break;
        };

        //перерасчет координат головы и смещения(только в режиме без границ) 
        hd = snake.head(); 
        if(twoplayers) hd2 = snake2.head();


        if (borderless) {
            hd += delta;
            deltaBorder = SCoord(0, 0);
            if (hd.x == 0) deltaBorder.x += width - 2;
            if (hd.x == width - 1) deltaBorder.x -= width - 2;
            if (hd.y == 0) deltaBorder.y += height - 2;
            if (hd.y == height - 1) deltaBorder.y -= height - 2;
            hd += deltaBorder;            
            if (twoplayers) {                
                hd2 += delta2;

                deltaBorder2 = SCoord(0,0);
                if (hd2.x == 0) deltaBorder2.x += width - 2;
                if (hd2.x == width - 1) deltaBorder2.x -= width - 2;
                if (hd2.y == 0) deltaBorder2.y += height - 2;
                if (hd2.y == height - 1) deltaBorder2.y -= height - 2;
                hd2 += deltaBorder2;
            }
        }
        else {
            hd += delta;
            if (twoplayers) hd2 += delta2;
        }
        
        //проверяем не врезалась ли змейка 1 в предмет
        if ((!borderless && (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1)) || snake.into(hd) || (twoplayers && (snake2.into(hd) || hd==hd2)) )
            stt = State::ST_DIED;
        //проверяем не врезалась ли змейка 2 в предмет
        if (twoplayers && ((!borderless && (hd2.x == 0 || hd2.x == width - 1 || hd2.y == 0 || hd2.y == height - 1)) || snake2.into(hd2) ||  snake.into(hd2) || hd == hd2))
            stt2 = State::ST_DIED;
        
        if (stt == State::ST_OK && stt2 == State::ST_OK) {
            //если змейка жива передвинуть змейку
             snake.move(delta + deltaBorder, scr);  
             if(twoplayers) snake2.move(delta2 + deltaBorder2, scr);
            //проверить не попала ли змейка в одну клетку с едой
            for (int i = 0; i < FoodOnMap; i++) {
                SCoord &food = foodVect[i];
                if (snake.head() == food) { 
                    //если змея съела еду увеличить змею, создать новую еду
                    snake.grow(food, SegmentsGet);
                    food = make_food();  
                    scr.pos(food.x, food.y, FOOD, foodColor);

                    //перерассчет времени и рейтинга.
                    time2 = clock();
                    duration = time2 - time1;
                    duration_game += double(duration) / CLOCKS_PER_SEC;
                    rating += double(snake.size()) / duration * CLOCKS_PER_SEC;
                    if (rating > rating_max) rating_max = rating;
                    time1 = time2;
                    print_stat();

                }
                if (twoplayers && snake2.head() == food) {
                    //если змея съела еду увеличить змею, создать новую еду
                    snake2.grow(food, SegmentsGet);
                    food = make_food();
                    scr.pos(food.x, food.y, FOOD, foodColor);

                    //перерассчет времени и рейтинга.
                    time2s2 = clock();
                    duration = time2s2 - time1s2;
                    duration_game2 += double(duration) / CLOCKS_PER_SEC;
                    rating2 += double(snake2.size()) / duration * CLOCKS_PER_SEC;
                    if (rating2 > rating_max2) rating_max2 = rating2;
                    time1s2 = time2s2;
                    print_stat();

                }
                
            }
            Sleep(latency);             // задержка перед следующим изменением позиции
        }
        else if (twoplayers) {
            if (stt == State::ST_OK && rating >= rating2) {
                scr.pos_str(width / 2 - 6, height / 2 - 4, "Player 1 Wins", txtColor);
                winsPlayer1++;
            }
            else if (stt2 == State::ST_OK && rating2 >= rating && stt != State::ST_EXIT) {
                scr.pos_str(width / 2 - 6, height / 2 - 4, "Player 2 Wins", txtColor);
                winsPlayer2++;
            }
            else if(stt!=State::ST_EXIT) scr.pos_str(width / 2 - 1, height / 2 - 4, "Tie", txtColor);

        }
    } while (stt == State::ST_OK && stt2 == State::ST_OK);          // играем пока змейка жива
    //выводим сообщение об окончании игры
    scr.pos_str(width / 2 - 8, height/2-1, "G a m e   o v e r", txtColor);
    clearkeys();
    _getch();
    clearkeys();
}