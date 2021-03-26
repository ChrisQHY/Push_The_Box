#pragma once
#include <iostream>
#include <windows.h>                //�������̨����
#include <conio.h>                  //�����ȡ���򰴼�

typedef enum {
    CELL_NULL = 0,                  //��������
    CELL_WALL,                      //ǽ
    CELL_PERSON,                    //��վ�ڿ�������
    CELL_PERSON_WITH_TARGET,        //��վ��Ŀ������
    CELL_BOX_TARGET,                //Ŀ������
    CELL_BOX_SHIFT,                 //���ӣ���������
    CELL_BOX_COMPLITE,              //���ӣ�Ŀ������
}cell_type;

typedef enum {
    MOVE_W = 0,                     //�����ƶ�
    MOVE_S,                         //�����ƶ�
    MOVE_A,                         //�����ƶ�
    MOVE_D,                         //�����ƶ�
}move_dir;

typedef struct {
    char x;                         //����������
    char y;                         //����������
}person_pos;

class game {
private:
    char row;                       //��Ϸ��������
    char column;                    //��Ϸ��������
    cell_type **map;                //��Ϸ�����ڴ��ַ
    person_pos p_pos;               //�˵�λ������

public:                             //������ϸ���ܲ鿴����λ��ע�͡�
    game();
    ~game();

    void draw_map();
    void action_map();
    bool finish_game();
    bool touch();
    bool move(move_dir dir, bool target_flag);
};

