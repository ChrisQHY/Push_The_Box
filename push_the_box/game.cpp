#include "game.h"

game::game()
{
    //初始化背景。
    row = 8;
    column = 10;

    //由于游戏区域为二维数组，首先申请连续的一维数组指针作为二维数组的第二维坐标。
    map = (cell_type **)malloc(row * sizeof(cell_type *));
    //之后对一维数组指针作为二维数组的第二维坐标进行申请连续的一维数组空间。
    for ( int i = 0; i < row; i++ ) {
        map[i] = (cell_type *)malloc(column * sizeof(cell_type));
    }

    p_pos.x = 5;
    p_pos.y = 5;
    for ( int i = 0; i < row; i++ ) {
        for ( int j = 0; j < column; j++ ) {
            if ( i == 0 || i == row - 1 ) {
                map[i][j] = CELL_WALL;
            } else if ( j == 0 || j == column - 1 ) {
                map[i][j] = CELL_WALL;
            } else {
                map[i][j] = CELL_NULL;
            }
        }
    }

    map[p_pos.y][p_pos.x] = CELL_PERSON;        //人
    map[4][4] = CELL_BOX_TARGET;                //目标区域A
    map[4][5] = CELL_BOX_TARGET;                //目标区域B
    map[3][4] = CELL_BOX_SHIFT;                 //箱子A
    map[3][5] = CELL_BOX_SHIFT;                 //箱子B

    //开始游戏。
    while ( true ) {
        draw_map();
        if ( finish_game() == true ) {
            break;
        }
        action_map();
    }
}

game::~game()
{
    //首先释放一维数组指针作为二维数组的第二维坐标进行申请连续的一维数组空间。
    for ( int i = 0; i < row; i++ ) {
        free(map[i]);
    }
    //最后释放一维数组指针作为二维数组的第二维坐标。
    free(map);
}

void game::draw_map()
{
    //关闭光标显示并且将光标位置归位到起始位置。
    CONSOLE_CURSOR_INFO cmd_info;
    COORD cmd_pos = { 0, 0 };

    cmd_info.bVisible = 0;
    cmd_info.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cmd_info);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cmd_pos);

    for ( int i = 0; i < row; i++ ) {
        for ( int j = 0; j < column; j++ ) {
            switch ( map[i][j] ) {
            case CELL_NULL:
                std::cout << ' ';
                break;
            case CELL_WALL:
                std::cout << 'W';
                break;
            case CELL_PERSON:
                std::cout << 'P';
                break;
            case CELL_PERSON_WITH_TARGET:
                std::cout << 'P';
                break;
            case CELL_BOX_TARGET:
                std::cout << 'X';
                break;
            case CELL_BOX_SHIFT:
                std::cout << 'B';
                break;
            case CELL_BOX_COMPLITE:
                std::cout << 'O';
                break;
            default:
                break;
            }
        }
        std::cout << '\n';
    }
}

void game::action_map()
{
    char command;
    bool is_with_target;
    bool touch_rst;
    bool move_rst;

    //此函数仅做移动处理，推动处理在move函数内进行逻辑处理。
    command = _getch();
    is_with_target = false;
    switch ( command ) {
    case 'w':
        //进行判断当前人是否站在目标区域。
        if ( map[p_pos.y][p_pos.x] == CELL_PERSON_WITH_TARGET ) {
            map[p_pos.y][p_pos.x] = CELL_BOX_TARGET;
            is_with_target = true;
        } else if ( map[p_pos.y][p_pos.x] == CELL_PERSON ) {
            map[p_pos.y][p_pos.x] = CELL_NULL;
        }
        //操控后如果移动的目的地时目标区域，则直接移动。
        if ( map[p_pos.y - 1][p_pos.x] == CELL_BOX_TARGET ) {
            p_pos.y--;
            map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            break;
        }
        p_pos.y--;
        //判断操作后移动的目的地是否有碰触到任何物品。
        touch_rst = touch();
        if ( touch_rst == true ) {
            //碰触到物品后是否可以进行推动箱子。
            move_rst = move(MOVE_W, is_with_target);
            if ( move_rst == true ) {
                //推动箱子完成后自动退出
                break;
            }
            //碰触物品且无法移动，则返回原位置。
            p_pos.y++;
        }
        //更新人在地图上的位置。
        map[p_pos.y][p_pos.x] = CELL_PERSON;
        break;

    case 's':
        if ( map[p_pos.y][p_pos.x] == CELL_PERSON_WITH_TARGET ) {
            map[p_pos.y][p_pos.x] = CELL_BOX_TARGET;
            is_with_target = true;
        } else if ( map[p_pos.y][p_pos.x] == CELL_PERSON ) {
            map[p_pos.y][p_pos.x] = CELL_NULL;
        }
        if ( map[p_pos.y + 1][p_pos.x] == CELL_BOX_TARGET ) {
            p_pos.y++;
            map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            break;
        }
        p_pos.y++;
        touch_rst = touch();
        if ( touch_rst == true ) {
            move_rst = move(MOVE_S, is_with_target);
            if ( move_rst == true ) {
                break;
            }
            p_pos.y--;
        }
        map[p_pos.y][p_pos.x] = CELL_PERSON;
        break;

    case 'a':
        if ( map[p_pos.y][p_pos.x] == CELL_PERSON_WITH_TARGET ) {
            map[p_pos.y][p_pos.x] = CELL_BOX_TARGET;
            is_with_target = true;
        } else if ( map[p_pos.y][p_pos.x] == CELL_PERSON ) {
            map[p_pos.y][p_pos.x] = CELL_NULL;
        }
        if ( map[p_pos.y][p_pos.x - 1] == CELL_BOX_TARGET ) {
            p_pos.x--;
            map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            break;
        }
        p_pos.x--;
        touch_rst = touch();
        if ( touch_rst == true ) {
            move_rst = move(MOVE_A, is_with_target);
            if ( move_rst == true ) {
                break;
            }
            p_pos.x++;
        }
        map[p_pos.y][p_pos.x] = CELL_PERSON;
        break;

    case 'd':
        if ( map[p_pos.y][p_pos.x] == CELL_PERSON_WITH_TARGET ) {
            map[p_pos.y][p_pos.x] = CELL_BOX_TARGET;
            is_with_target = true;
        } else if ( map[p_pos.y][p_pos.x] == CELL_PERSON ) {
            map[p_pos.y][p_pos.x] = CELL_NULL;
        }
        if ( map[p_pos.y][p_pos.x + 1] == CELL_BOX_TARGET ) {
            p_pos.x++;
            map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            break;
        }
        p_pos.x++;
        touch_rst = touch();
        if ( touch_rst == true ) {
            move_rst = move(MOVE_D, is_with_target);
            if ( move_rst == true ) {
                break;
            }
            p_pos.x--;
        }
        map[p_pos.y][p_pos.x] = CELL_PERSON;
        break;
    }
}

bool game::finish_game()
{
    char count;

    //当内部不存在空余箱子时游戏结束。
    count = 0;
    for ( int i = 0; i < row; i++ ) {
        for ( int j = 0; j < column; j++ ) {
            if ( map[i][j] == CELL_BOX_SHIFT ) {
                count++;
            }
        }
    }

    if ( count == 0 ) {
        std::cout << "GAME OVER!\n";
        return true;
    }

    return false;
}

bool game::touch()
{
    //用于action函数的碰撞处理。
    if ( map[p_pos.y][p_pos.x] != CELL_NULL ) {
        return true;
    } else {
        return false;
    }
}

bool game::move(move_dir dir, bool target_flag)
{
    //用于处理移动箱子的相关逻辑。
    if ( map[p_pos.y][p_pos.x] != CELL_BOX_SHIFT &&
        map[p_pos.y][p_pos.x] != CELL_BOX_COMPLITE ) {
        //如果碰触的不是箱子，移动失败。（CELL_BOX_COMPLITE是指箱子在目的区域）
        return false;
    }
    switch ( dir ) {
    case MOVE_W:
        //判断推动箱子的目的地是空闲区域还是目的区域，如果都不是，移动失败。
        if ( map[p_pos.y - 1][p_pos.x] == CELL_NULL ||
            map[p_pos.y - 1][p_pos.x] == CELL_BOX_TARGET ) {
            //判断人移动前所站位置是空闲区域还是目的区域，进行还原。
            if ( target_flag == true ) {
                map[p_pos.y + 1][p_pos.x] = CELL_BOX_TARGET;
            } else {
                map[p_pos.y + 1][p_pos.x] = CELL_NULL;
            }
            //判断人移动后所站位置是空闲区域还是目的区域，进行保留。
            if ( map[p_pos.y][p_pos.x] == CELL_BOX_COMPLITE ) {
                map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            } else {
                map[p_pos.y][p_pos.x] = CELL_PERSON;
            }
            //判断箱子推动后是空闲区域还是目的区域，进行显示。
            if ( map[p_pos.y - 1][p_pos.x] == CELL_NULL ) {
                map[p_pos.y - 1][p_pos.x] = CELL_BOX_SHIFT;
            } else {
                map[p_pos.y - 1][p_pos.x] = CELL_BOX_COMPLITE;
            }
        } else {
            return false;
        }
        break;

    case MOVE_S:
        if ( map[p_pos.y + 1][p_pos.x] == CELL_NULL ||
            map[p_pos.y + 1][p_pos.x] == CELL_BOX_TARGET ) {
            if ( target_flag == true ) {
                map[p_pos.y - 1][p_pos.x] = CELL_BOX_TARGET;
            } else {
                map[p_pos.y - 1][p_pos.x] = CELL_NULL;
            }
            if ( map[p_pos.y][p_pos.x] == CELL_BOX_COMPLITE ) {
                map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            } else {
                map[p_pos.y][p_pos.x] = CELL_PERSON;
            }
            if ( map[p_pos.y + 1][p_pos.x] == CELL_NULL ) {
                map[p_pos.y + 1][p_pos.x] = CELL_BOX_SHIFT;
            } else {
                map[p_pos.y + 1][p_pos.x] = CELL_BOX_COMPLITE;
            }
        } else {
            return false;
        }
        break;

    case MOVE_A:
        if ( map[p_pos.y][p_pos.x - 1] == CELL_NULL ||
            map[p_pos.y][p_pos.x - 1] == CELL_BOX_TARGET ) {
            if ( target_flag == true ) {
                map[p_pos.y][p_pos.x + 1] = CELL_BOX_TARGET;
            } else {
                map[p_pos.y][p_pos.x + 1] = CELL_NULL;
            }
            if ( map[p_pos.y][p_pos.x] == CELL_BOX_COMPLITE ) {
                map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            } else {
                map[p_pos.y][p_pos.x] = CELL_PERSON;
            }
            if ( map[p_pos.y][p_pos.x - 1] == CELL_NULL ) {
                map[p_pos.y][p_pos.x - 1] = CELL_BOX_SHIFT;
            } else {
                map[p_pos.y][p_pos.x - 1] = CELL_BOX_COMPLITE;
            }
        } else {
            return false;
        }
        break;

    case MOVE_D:
        if ( map[p_pos.y][p_pos.x + 1] == CELL_NULL ||
            map[p_pos.y][p_pos.x + 1] == CELL_BOX_TARGET ) {
            if ( target_flag == true ) {
                map[p_pos.y][p_pos.x - 1] = CELL_BOX_TARGET;
            } else {
                map[p_pos.y][p_pos.x - 1] = CELL_NULL;
            }
            if ( map[p_pos.y][p_pos.x] == CELL_BOX_COMPLITE ) {
                map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            } else {
                map[p_pos.y][p_pos.x] = CELL_PERSON;
            }
            if ( map[p_pos.y][p_pos.x + 1] == CELL_NULL ) {
                map[p_pos.y][p_pos.x + 1] = CELL_BOX_SHIFT;
            } else {
                map[p_pos.y][p_pos.x + 1] = CELL_BOX_COMPLITE;
            }
        } else {
            return false;
        }
        break;
    }

    return true;
}
