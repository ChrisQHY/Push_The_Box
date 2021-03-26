#pragma once
#include <iostream>
#include <windows.h>                //负责控制台处理
#include <conio.h>                  //负责获取方向按键

typedef enum _cell_type {
    CELL_NULL = 0,                  //空闲区域
    CELL_WALL,                      //墙
    CELL_PERSON,                    //人站在空闲区域
    CELL_PERSON_WITH_TARGET,        //人站在目标区域
    CELL_BOX_TARGET,                //目标区域
    CELL_BOX_SHIFT,                 //箱子（空闲区域）
    CELL_BOX_COMPLITE,              //箱子（目标区域）
}cell_type;

typedef enum _move_dir {
    MOVE_W = 0,                     //向上移动
    MOVE_S,                         //向下移动
    MOVE_A,                         //向左移动
    MOVE_D,                         //向右移动
}move_dir;

typedef struct _person_pos {
    char x;                         //人坐标列数
    char y;                         //人坐标行数
}person_pos;

class game {
private:
    char row;                       //游戏区域行数
    char column;                    //游戏区域列数
    cell_type **map;                //游戏区域内存地址
    person_pos p_pos;               //人的位置坐标

public:                             //方法详细介绍查看定义位置注释。
    game();
    ~game();

    void draw_map();
    void action_map();
    bool finish_game();
    bool touch();
    bool move(move_dir dir, bool target_flag);
};

