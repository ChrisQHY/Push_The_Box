#include "game.h"

game::game()
{
    //��ʼ��������
    row = 8;
    column = 10;

    //������Ϸ����Ϊ��ά���飬��������������һά����ָ����Ϊ��ά����ĵڶ�ά���ꡣ
    map = (cell_type **)malloc(row * sizeof(cell_type *));
    //֮���һά����ָ����Ϊ��ά����ĵڶ�ά�����������������һά����ռ䡣
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

    map[p_pos.y][p_pos.x] = CELL_PERSON;        //��
    map[4][4] = CELL_BOX_TARGET;                //Ŀ������A
    map[4][5] = CELL_BOX_TARGET;                //Ŀ������B
    map[3][4] = CELL_BOX_SHIFT;                 //����A
    map[3][5] = CELL_BOX_SHIFT;                 //����B

    //��ʼ��Ϸ��
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
    //�����ͷ�һά����ָ����Ϊ��ά����ĵڶ�ά�����������������һά����ռ䡣
    for ( int i = 0; i < row; i++ ) {
        free(map[i]);
    }
    //����ͷ�һά����ָ����Ϊ��ά����ĵڶ�ά���ꡣ
    free(map);
}

void game::draw_map()
{
    //�رչ����ʾ���ҽ����λ�ù�λ����ʼλ�á�
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

    //�˺��������ƶ������ƶ�������move�����ڽ����߼�����
    command = _getch();
    is_with_target = false;
    switch ( command ) {
    case 'w':
        //�����жϵ�ǰ���Ƿ�վ��Ŀ������
        if ( map[p_pos.y][p_pos.x] == CELL_PERSON_WITH_TARGET ) {
            map[p_pos.y][p_pos.x] = CELL_BOX_TARGET;
            is_with_target = true;
        } else if ( map[p_pos.y][p_pos.x] == CELL_PERSON ) {
            map[p_pos.y][p_pos.x] = CELL_NULL;
        }
        //�ٿغ�����ƶ���Ŀ�ĵ�ʱĿ��������ֱ���ƶ���
        if ( map[p_pos.y - 1][p_pos.x] == CELL_BOX_TARGET ) {
            p_pos.y--;
            map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            break;
        }
        p_pos.y--;
        //�жϲ������ƶ���Ŀ�ĵ��Ƿ����������κ���Ʒ��
        touch_rst = touch();
        if ( touch_rst == true ) {
            //��������Ʒ���Ƿ���Խ����ƶ����ӡ�
            move_rst = move(MOVE_W, is_with_target);
            if ( move_rst == true ) {
                //�ƶ�������ɺ��Զ��˳�
                break;
            }
            //������Ʒ���޷��ƶ����򷵻�ԭλ�á�
            p_pos.y++;
        }
        //�������ڵ�ͼ�ϵ�λ�á�
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

    //���ڲ������ڿ�������ʱ��Ϸ������
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
    //����action��������ײ����
    if ( map[p_pos.y][p_pos.x] != CELL_NULL ) {
        return true;
    } else {
        return false;
    }
}

bool game::move(move_dir dir, bool target_flag)
{
    //���ڴ����ƶ����ӵ�����߼���
    if ( map[p_pos.y][p_pos.x] != CELL_BOX_SHIFT &&
        map[p_pos.y][p_pos.x] != CELL_BOX_COMPLITE ) {
        //��������Ĳ������ӣ��ƶ�ʧ�ܡ���CELL_BOX_COMPLITE��ָ������Ŀ������
        return false;
    }
    switch ( dir ) {
    case MOVE_W:
        //�ж��ƶ����ӵ�Ŀ�ĵ��ǿ���������Ŀ��������������ǣ��ƶ�ʧ�ܡ�
        if ( map[p_pos.y - 1][p_pos.x] == CELL_NULL ||
            map[p_pos.y - 1][p_pos.x] == CELL_BOX_TARGET ) {
            //�ж����ƶ�ǰ��վλ���ǿ���������Ŀ�����򣬽��л�ԭ��
            if ( target_flag == true ) {
                map[p_pos.y + 1][p_pos.x] = CELL_BOX_TARGET;
            } else {
                map[p_pos.y + 1][p_pos.x] = CELL_NULL;
            }
            //�ж����ƶ�����վλ���ǿ���������Ŀ�����򣬽��б�����
            if ( map[p_pos.y][p_pos.x] == CELL_BOX_COMPLITE ) {
                map[p_pos.y][p_pos.x] = CELL_PERSON_WITH_TARGET;
            } else {
                map[p_pos.y][p_pos.x] = CELL_PERSON;
            }
            //�ж������ƶ����ǿ���������Ŀ�����򣬽�����ʾ��
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
