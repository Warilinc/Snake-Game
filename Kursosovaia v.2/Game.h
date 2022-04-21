#pragma once
#include "ConsoleScreen.h"
#include "SCoord.h"
#include "Snake.h"
/// <summary>
/// ����� ����
/// </summary>
class CGame {
public:
    CGame(ConsoleScreen& _scr, int _width = 80, int _height = 24, int _latency = 120); //�����������
    void game_loop();           // �������� ���� ����
    bool once_more();           // ����� ������� � ���� ������ �� ������
    void pak(int y);            // "Press any key for continue"
    void Settings();            //��������� ����
    void print_stat();          // ���������� ����������
    void Logo();                //������� ���� ����
    void bye();                 //������� ��������


private:
    enum class Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN, CMD_PLAY, CMD_2LEFT, CMD_2RIGHT, CMD_2UP, CMD_2DOWN,}; //������� ����
    enum class State { ST_OK, ST_EXIT, ST_DIED };  //��������� ����

    typedef std::pair<int, Command> CmdPair;

    int width, height;          //������� ����
    int latency;                //�������� ����� ����������
    int SegmentsGet=3;          //����������� ���������� ������� ���������
    int FoodOnMap=1;            //����������� ��� �� ����
    bool borderless = true;    //����� ��� ������ �������/��������
    bool twoplayers = true;    //����� ��� ���� �������
    int winsPlayer1 = 0;        //������ 1 ������
    int winsPlayer2 = 0;        //������ 2 ������
    ConsoleScreen scr;                  //�������
    CSnake snake, snake2;               //������
    CmdPair cmd_table[10];              //������� �������
    double duration_game, duration_game2;               //������������ ����
    double rating, rating_max=0.0, rating2, rating_max2 = 0.0;      //������� � ������������ �������
    WORD foodColor = 15;                //���� ���
    WORD bdColor = 7;                   //���� ������ �������� ����
    WORD txtColor = 7;                  //���� ������
    void draw_field();                  //���������� ������� ����
    SCoord make_food();                 //������� ���
    SCoord foodVect[5]{SCoord()};       //������ ��������� ���
    Command get_command();              //�������� � ���������� ��������
};

