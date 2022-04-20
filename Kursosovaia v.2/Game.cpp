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

    for (int i = 0; i < 6; i++) {
        if (cmd_table[i].first == ch) {
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
    do {
        food.x = rand() % (width - 2) + 1;
        food.y = rand() % (height - 2) + 1;
        for (int i = 0; i < FoodOnMap; i++) if (food.x == foodVect[i].x && food.y == foodVect[i].y) continue;
    } while (snake.into(food));

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
                scr.pos(x, y, BORDER, bdtxtColor);
        }
        else {
            scr.pos(0, y, BORDER, bdtxtColor);
            scr.pos(width - 1, y, BORDER, bdtxtColor);
        }
    }
    scr.pos(0, height);
    _cprintf("Length: ****  Rating: ******.**** (******.****)  Time: ****.**");
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
    scr.setColor(bdtxtColor);
    scr.pos(8, height);
    _cprintf("%04u", snake.size()); //длина змейки
    scr.pos(22, height);
    _cprintf("%011.4f", rating); //рейтинг текущей игры
    scr.pos(35, height);
    _cprintf("%011.4f", rating_max); //максимальный рейтинг
    scr.pos(55, height);
    _cprintf("%07.2f", duration_game); //время игры
}

/// <summary>
/// спросить игрока хочет ли он сыграть еще раз
/// </summary>
/// <returns>да/нет</returns>
bool CGame::once_more() {
    scr.pos_str(width / 2 - 16, height - 3, "Are you want to play once more?", bdtxtColor);
    scr.pos_str(width / 2 - 6, height - 2, "[S]ettings", bdtxtColor);
    
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
    int setwidth = width;
    scr.pos_str(setwidth / 2, 1, "Settings:", 7);
    scr.pos_str(setwidth / 2, 9, "Colors:", 7);
    scr.pos_str(setwidth / 2, 19, "Difficulty:", 7);
    scr.pos_str(setwidth / 2, 29, "Press [Enter] to play", 7);

    Command cmd = Command::CMD_NOCOMMAND;
    int delta = 0;

    do {
        //печать настроек

        scr.pos(setwidth / 2, 4);
        scr.setColor(15);
        if (delta == 0)  
            _cprintf(" > Height: %03u < ", height);
        else
            _cprintf("   Height: %03u   ", height);

        scr.pos(setwidth / 2, 6);
        if (delta == 1)           
            _cprintf(" > Width: %03u < ", width);
        else
            _cprintf("   Width: %03u   ", width);
       
        if (delta == 2)
            scr.pos_str(setwidth / 2, 12, " > Snake: @@@% < ", snake.Color);
        else
            scr.pos_str(setwidth / 2, 12, "   Snake: @@@%   ", snake.Color);

        if (delta == 3)
            scr.pos_str(setwidth / 2, 14," > Fruit: * < ", foodColor);
        else
            scr.pos_str(setwidth / 2, 14,"   Fruit: *   ", foodColor);

        if (delta == 4)
            scr.pos_str(setwidth / 2, 16," > Border and Text: ### < ", bdtxtColor);
        else
            scr.pos_str(setwidth / 2, 16,"   Border and Text: ###   ", bdtxtColor);
        
        scr.setColor(15);

        scr.pos(setwidth / 2, 22);
        if (delta == 5)
            _cprintf(" > Segments per eat: %u < ", SegmentsGet);
        else
            _cprintf("   Segments per eat: %u   ", SegmentsGet);

        scr.pos(setwidth / 2, 24);
        if (delta == 6)
            _cprintf(" > Food on map: %u < ", FoodOnMap);
        else
            _cprintf("   Food on map: %u   ", FoodOnMap);

        scr.pos(setwidth / 2, 26);
        if (delta == 7)
            
            _cprintf(" > Borderless mode: %s < ", (borderless ? "On" : "Off"));
        else
            _cprintf("   Borderless mode: %s   ", (borderless ? "On" : "Off"));

        cmd = get_command();

        // обработка команд
        switch (cmd) {
        case Command::CMD_LEFT:
            if (delta == 0 && height>10) height -= 1;
            if (delta == 1 && width> 40) width -= 1;
            if (delta == 2 && snake.Color < 15) snake.Color += 1;
            if (delta == 3 && foodColor < 15) foodColor += 1;
            if (delta == 4 && bdtxtColor < 15) bdtxtColor += 1;
            if (delta == 5 && SegmentsGet > 1) SegmentsGet -= 1;
            if (delta == 6 && FoodOnMap > 1) FoodOnMap -= 1;
            if (delta == 7) borderless = !borderless;
            break;
        case Command::CMD_RIGHT:
            if (delta == 0) height += 1;
            if (delta == 1) width += 1;
            if (delta == 2 && snake.Color > 1) snake.Color -= 1;
            if (delta == 3 && foodColor > 1) foodColor -= 1;
            if (delta == 4 && bdtxtColor > 1) bdtxtColor -= 1;
            if (delta == 5 && SegmentsGet < 5) SegmentsGet += 1;
            if (delta == 6 && FoodOnMap < 5) FoodOnMap += 1;
            if (delta == 7) borderless = !borderless;
            break;
        case Command::CMD_UP:
            if (delta > 0)
                delta -= 1;
            break;
        case Command::CMD_DOWN:
            if (delta < 7 )
                delta += 1;
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
    snake.reset(SCoord(width / 2, height / 2));
    duration_game = 0;
    rating = 0.0;
    Command cmd = Command::CMD_NOCOMMAND;
    State stt = State::ST_OK;
    SCoord delta(-1, 0);  

    //нарисовать начальную еду и змейку
    for (int i = 0; i < FoodOnMap; i++) {
        foodVect[i] = make_food();          
        scr.pos(foodVect[i].x, foodVect[i].y, FOOD, foodColor);             
    }
    snake.print(scr);                    
    print_stat();                       
    clock_t time1, time2, duration;
    time1 = clock();

    do {
        // если в буфере клавиатуры есть информация, то принять команду
        if (_kbhit())
            cmd = get_command();      

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

        //перерасчет координат головы и смещения(только в режиме без границ) 
        SCoord hd = snake.head(); 
        if (borderless) {
            hd += delta;
            SCoord deltaBorder(0, 0);
            if (hd.x == 0) deltaBorder.x += width - 2;
            if (hd.x == width - 1) deltaBorder.x -= width - 2;
            if (hd.y == 0) deltaBorder.y += height - 2;
            if (hd.y == height - 1) deltaBorder.y -= height - 2;
            hd += deltaBorder;
            delta += deltaBorder;            
        }
        else {
            hd += delta;
        }
        
        //проверяем не врезалась ли змейка в предмет
        if ((!borderless && (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1)) || snake.into(hd))
            stt = State::ST_DIED;
        
        if (stt == State::ST_OK) {     
            //если змейка жива передвинуть змейку
             snake.move(delta, scr);  

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
            }
            Sleep(latency);             // задержка перед следующим изменением позиции
        }
    } while (stt == State::ST_OK);          // играем пока змейка жива
    //выводим сообщение об окончании игры
    scr.pos_str(width / 2 - 9, height/2-1, "G a m e   o v e r", bdtxtColor);
    clearkeys();
    _getch();
    clearkeys();
}