#include "SkiFree.h"

#include <cassert>
#include <cstdio>
#include <raymath.h>


void SkiObject::update(const std::vector<Rectangle> &frames, const float speed_limit)
{
    switch (type)
    {
        case TYPE_SKIER:
        {
            switch (state)
            {
                case STATE_PLAYER_30_LEFT:
                case STATE_PLAYER_30_RIGHT:
                {
                    speed += 0.3;
                    break;
                }
                case STATE_PLAYER_60_LEFT:
                case STATE_PLAYER_60_RIGHT:
                {
                    speed += 0.6;
                    break;
                }
                case STATE_PLAYER_DOWN:
                {
                    speed += 1;
                    break;
                }
                case STATE_PLAYER_LEFT:
                case STATE_PLAYER_RIGHT:
                {
                    if (speed > 0)
                    {
                        speed -= 5;
                    }
                    break;
                }
                case STATE_PLAYER_HIT:
                {
                    if (offset_y == 0)
                    {
                        speed = 0;
                        state = STATE_PLAYER_OUCH;
                        current_frame_index = 11;
                        current_frame_rectangle = frames[11];
                        state_countdown = 10;
                    }
                    else
                    {
                        offset_y -= 1;
                    }
                    break;
                }
                case STATE_PLAYER_OUCH:
                {
                    --state_countdown;
                    if (state_countdown == 0)
                    {
                        state = STATE_PLAYER_SIT;
                        current_frame_index = 12;
                        current_frame_rectangle = frames[12];
                    }
                    break;
                }
                case STATE_PLAYER_JUMP:
                {
                    if (jump > 0)
                    {
                        ++offset_y;
                        --jump;
                    }
                    else
                    {
                        --offset_y;
                    }
                    if (offset_y < 0.001)
                    {
                        offset_y = 0;
                        jump = 0;
                        if (current_frame_index == 13)
                        {
                            state = STATE_PLAYER_DOWN;
                            current_frame_index = 0;
                            current_frame_rectangle = frames[0];
                        }
                        else if (current_frame_index == 14)
                        {
                            state = STATE_PLAYER_LEFT;
                            current_frame_index = 3;
                            current_frame_rectangle = frames[3];
                        }
                        else if (current_frame_index == 15)
                        {
                            state = STATE_PLAYER_RIGHT;
                            current_frame_index = 6;
                            current_frame_rectangle = frames[6];
                        }
                        else
                        {
                            speed = 0;
                            state = STATE_PLAYER_OUCH;
                            current_frame_index = 11;
                            current_frame_rectangle = frames[11];
                            state_countdown = 10;
                        }
                    }

                    break;
                }
                default:
                    break;
            }
            if (speed > speed_limit)
            {
                speed = speed_limit;
                if (offset_y > 0)
                {
                    speed += 2;
                }
            }

            if (std::abs(speed) < 0.001)
            {
                speed = 0;
            }
            speed = std::max(speed, 0.0);
            break;
        }
        case TYPE_YETI:
        {
            switch (state)
            {
                case STATE_YETI_HAPPY_1:
                {
                    --state_countdown;
                    if (state_countdown <= 0)
                    {
                        state = STATE_YETI_HAPPY_2;
                        current_frame_index = 68;
                        current_frame_rectangle = frames[current_frame_index];
                        state_countdown = 9;
                        offset_y = 0;
                    }
                    break;
                };
                case STATE_YETI_HAPPY_2:
                {
                    --state_countdown;
                    if (state_countdown == 9)
                    {
                        offset_y = 0;
                    }
                    else if (state_countdown == 8 | state_countdown == 1)
                    {
                        offset_y = 5;
                    }
                    else if (state_countdown == 7 | state_countdown == 2)
                    {
                        offset_y = 10;
                    }
                    else if (state_countdown == 6 | state_countdown == 3)
                    {
                        offset_y = 15;
                    }
                    else if (state_countdown == 5 | state_countdown == 4)
                    {
                        offset_y = 20;
                    }
                    else
                    {
                        state = STATE_YETI_HAPPY_1;
                        current_frame_index = 67;
                        current_frame_rectangle = frames[current_frame_index];
                        state_countdown = GetRandomValue(1, 25);
                        offset_y = 0;
                    }
                    break;
                }
                case STATE_YETI_LEFT:
                {
                    if (current_frame_index == 69)
                    {
                        current_frame_index = 70;
                    }
                    else
                    {
                        current_frame_index = 69;
                    }
                    current_frame_rectangle = frames[current_frame_index];
                    break;
                }
                case STATE_YETI_RIGHT:
                {
                    if (current_frame_index == 71)
                    {
                        current_frame_index = 72;
                    }
                    else
                    {
                        current_frame_index = 71;
                    }
                    current_frame_rectangle = frames[current_frame_index];
                    break;
                }
                case STATE_YETI_UP:
                {
                    if (current_frame_index == 73)
                    {
                        current_frame_index = 74;
                    }
                    else
                    {
                        current_frame_index = 73;
                    }
                    current_frame_rectangle = frames[current_frame_index];
                    break;
                }
                case STATE_YETI_EATING:
                {
                    --state_countdown;
                    current_frame_index = 81 - state_countdown;
                    current_frame_rectangle = frames[current_frame_index];
                    if (state_countdown == 0)
                    {
                        state = STATE_YETI_HAPPY_1;
                        current_frame_index = 67;
                        current_frame_rectangle = frames[current_frame_index];
                        state_countdown = GetRandomValue(1, 25);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        case TYPE_NOVICE:
        {
            switch (state)
            {
                case STATE_NOVICE_NORMAL:
                {
                    --state_countdown;
                    if (state_countdown == 0)
                    {
                        int dir = GetRandomValue(-1, 1);
                        if (dir == -1)
                        {
                            direction.x = cosf(120 * DEG2RAD);
                            current_frame_index = 28;
                        }
                        else if (dir == 0)
                        {
                            direction.x = cosf(90 * DEG2RAD);
                            current_frame_index = 27;
                        }
                        else
                        {
                            direction.x = cosf(60 * DEG2RAD);
                            current_frame_index = 29;
                        }
                        current_frame_rectangle = frames[current_frame_index];
                        state_countdown = GetRandomValue(4, 30);
                    }
                    break;
                }

                default:
                    break;
            }
            break;
        }
        case TYPE_DOG:
        {
            switch (state)
            {
                case STATE_DOG_RIGHT:
                {
                    --state_countdown;
                    if (state_countdown == 0)
                    {
                        direction.y *= -1;
                        state_countdown = 2;
                    }
                    current_frame_index = 32 + (state_countdown % 2);
                    current_frame_rectangle = frames[current_frame_index];
                    break;
                }
                case STATE_DOG_HIT:
                {
                    --state_countdown;
                    current_frame_index = 34 + (state_countdown % 2);
                    current_frame_rectangle = frames[current_frame_index];
                    if (state_countdown == 0)
                    {
                        state = STATE_DOG_RIGHT;
                        state_countdown = 2;
                        speed = 2;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case TYPE_SNOWBOARDER:
        {
            --state_countdown;
            switch (state)
            {
                case STATE_SNOWBOARDER_360:
                {
                    current_frame_index = 44 - state_countdown;
                    current_frame_rectangle = frames[current_frame_index];
                    break;
                }
                default:
                {
                    break;
                }
            }

            if (state_countdown == 0)
            {
                int new_state = GetRandomValue(0, 2);
                if (new_state == 0)
                {
                    current_frame_index = 36;
                    direction.x = -1;
                    state = STATE_SNOWBOARDER_LEFT;
                    state_countdown = 10;
                }
                else if (new_state == 1)
                {
                    current_frame_index = 37;
                    direction.x = 1;
                    state = STATE_SNOWBOARDER_RIGHT;
                    state_countdown = 10;
                }
                else
                {
                    state = STATE_SNOWBOARDER_360;
                    state_countdown = 6;
                    current_frame_index = 44 - state_countdown;
                }
                current_frame_rectangle = frames[current_frame_index];
            }
            break;
        }
        case TYPE_TREE_WALK:
        {
            --state_countdown;
            if (state_countdown == 0)
            {
                direction.x *= -1;
                state_countdown = 36;
                speed = 2;
            }
            else if (state_countdown < 18)
            {
                current_frame_index = 48;
                current_frame_rectangle = frames[current_frame_index];
                speed = 0;
            }

            if (speed > 0)
            {
                if (state_countdown % 2 == 0)
                {
                    current_frame_index = 86;
                }
                else
                {
                    if (direction.x > 0)
                    {
                        current_frame_index = 87;
                    }
                    else
                    {
                        current_frame_index = 88;
                    }
                }
                current_frame_rectangle = frames[current_frame_index];
            }
            break;
        }
        case TYPE_LIFT_CHAIR:
        {
            if (position.y < -65 * 20)
            {
                state = STATE_LIFT_CHAIR_EMPTY;
                position.x = -107;
                current_frame_index = 66;
                current_frame_rectangle = frames[66];
                direction.y = 1;
            }
            else if (position.y > 1471 * 20)
            {
                state = STATE_LIFT_CHAIR_FULL;
                position.x = -73;
                current_frame_index = 64;
                current_frame_rectangle = frames[64];
                direction.y = -1;
            }
            if (state_countdown > 0)
            {
                --state_countdown;
            }

            break;
        }
        case TYPE_TREE_DRIED:
        {
            if (state == STATE_TREE_DRIED_FIRE)
            {
                state_countdown = (state_countdown + 1) % 3;
                current_frame_index = 82 + state_countdown;
                current_frame_rectangle = frames[current_frame_index];
            }
            break;
        }
        default:
            break;
    }

    if (direction.x != 0 || direction.y != 0)
    {
        velocity = direction * speed;
    }

    position += velocity;
}

Rectangle SkiObject::get_location() const
{
    return Rectangle(
            position.x, position.y, current_frame_rectangle.width, current_frame_rectangle.height);
}

BoundingBox SkiObject::get_collision_box() const
{
    BoundingBox result{};

    switch (type)
    {
        case TYPE_SKIER:
        {
            switch (state)
            {
                case STATE_PLAYER_LEFT:
                case STATE_PLAYER_RIGHT:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y + 23;
                    result.max.x = result.min.x + 24;
                    result.max.y = result.min.y + 28;
                    result.max.z = result.min.z + 28 - 23;
                    break;
                }
                case STATE_PLAYER_60_LEFT:
                case STATE_PLAYER_60_RIGHT:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y + 16;
                    result.max.x = result.min.x + 24;
                    result.max.y = result.min.y + 28;
                    result.max.z = result.min.z + 28 - 16;
                    break;
                }
                case STATE_PLAYER_30_LEFT:
                case STATE_PLAYER_30_RIGHT:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y + 15;
                    result.max.x = result.min.x + 16;
                    result.max.y = result.min.y + 28;
                    result.max.z = result.min.z + 32 - 15;
                    break;
                }
                case STATE_PLAYER_DOWN:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y + 15;
                    result.max.x = result.min.x + 16;
                    result.max.y = result.min.y + 4;
                    result.max.z = result.min.z + 32 - 15;
                    break;
                }
                case STATE_PLAYER_JUMP:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y + 20;
                    result.max.x = result.min.x + 30;
                    result.max.y = result.min.y + 32;
                    result.max.z = result.min.z + 32 - 20;
                    break;
                }
                case STATE_PLAYER_HIT:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y;
                    result.max.x = result.min.x + 26;
                    result.max.y = result.min.y + 32;
                    result.max.z = result.min.z + 32;
                    break;
                }
                case STATE_PLAYER_SIT:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y;
                    result.max.x = result.min.x + 24;
                    result.max.y = result.min.y + 32;
                    result.max.z = result.min.z + 32;
                    break;
                }
                case STATE_PLAYER_OUCH:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y;
                    result.max.x = result.min.x + 32;
                    result.max.y = result.min.y + 32;
                    result.max.z = result.min.z + 32;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case TYPE_YETI:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 32;
            result.max.y = result.min.y + 48;
            result.max.z = result.min.z + 32;
            break;
        }
        case TYPE_LIFT_POLE:
        {
            result.min.x = position.x + 7;
            result.min.y = offset_y;
            result.min.z = position.y + 56;
            result.max.x = result.min.x + 9;
            result.max.y = result.min.y + 64;
            result.max.z = result.min.z + 9;
            break;
        }
        case TYPE_LIFT_CHAIR:
        {
            result.min.x = position.x;
            result.min.y = 32;
            result.min.z = position.y;
            result.max.x = result.min.x + 26;
            result.max.y = result.min.y + 32;
            result.max.z = result.min.z + 26;
            break;
        }
        case TYPE_URINE:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 16;
            result.max.y = result.min.y + 1;
            result.max.z = result.min.z + 8;
            break;
        }
        case TYPE_MOGUL_GROUP:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 64;
            result.max.y = result.min.y + 6;
            result.max.z = result.min.z + 32;
            break;
        }
        case TYPE_NOVICE:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y + 16;
            result.max.x = result.min.x + 24;
            result.max.y = result.min.y + 30;
            result.max.z = result.min.z + 30 - 16;
            break;
        }
        case TYPE_DOG:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y + 10;
            result.max.x = result.min.x + 20;
            result.max.y = result.min.y + 10;
            result.max.z = result.min.z + 15;
            break;
        }
        case TYPE_SNOWBOARDER:
        {
            switch (state)
            {
                case STATE_SNOWBOARDER_LEFT:
                case STATE_SNOWBOARDER_RIGHT:
                {
                    result.min.x = position.x + 5;
                    result.min.y = offset_y;
                    result.min.z = position.y + 5;
                    result.max.x = result.min.x + 15;
                    result.max.y = result.min.y + 30;
                    result.max.z = result.min.z + 15;
                    break;
                }
                case STATE_SNOWBOARDER_360:
                {
                    result.min.x = position.x;
                    result.min.y = offset_y;
                    result.min.z = position.y;
                    result.max.x = result.min.x + 20;
                    result.max.y = result.min.y + 30;
                    result.max.z = result.min.z + 20;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case TYPE_TREE_WALK:
        case TYPE_TREE_SMALL:
        case TYPE_TREE_DRIED:
        {
            result.min.x = position.x + 7;
            result.min.y = offset_y;
            result.min.z = position.y + 25;
            result.max.x = result.min.x + 7;
            result.max.y = result.min.y + 32;
            result.max.z = result.min.z + 7;
            break;
        }
        case TYPE_ROCK:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y + 2;
            result.max.x = result.min.x + 23;
            result.max.y = result.min.y + 11;
            result.max.z = result.min.z + 11;
            break;
        }
        case TYPE_STUMP:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y + 2;
            result.max.x = result.min.x + 16;
            result.max.y = result.min.y + 11;
            result.max.z = result.min.z + 11;
            break;
        }
        case TYPE_MOGUL_SMALL:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 16;
            result.max.y = result.min.y + 4;
            result.max.z = result.min.z + 4;
            break;
        }
        case TYPE_MOGUL_LARGE:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 24;
            result.max.y = result.min.y + 8;
            result.max.z = result.min.z + 8;
            break;
        }
        case TYPE_TREE_LARGE:
        {
            result.min.x = position.x + 10;
            result.min.y = offset_y;
            result.min.z = position.y + 55;
            result.max.x = result.min.x + 12;
            result.max.y = result.min.y + 64;
            result.max.z = result.min.z + 9;
            break;
        }
        case TYPE_RAINBOW:
        {
            result.min.x = position.x;
            result.min.y = offset_y;
            result.min.z = position.y;
            result.max.x = result.min.x + 32;
            result.max.y = result.min.y + 8;
            result.max.z = result.min.z + 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return result;
}

SkiFree::SkiFree()
{
    all_textures = LoadTexture("assets/images.png");
    frames.resize(89);
    int prev_y = 0;
    frames[0] = Rectangle(0, prev_y += 0, 16, 32); // skier_down
    frames[1] = Rectangle(0, prev_y += 32, 16, 32); // skier_30_left
    frames[2] = Rectangle(0, prev_y += 32, 24, 28); // skier_60_left
    frames[3] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_0
    frames[4] = Rectangle(0, prev_y += 28, 16, 32); // skier_30_right
    frames[5] = Rectangle(0, prev_y += 32, 24, 28); // skier_60_right
    frames[6] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_0
    frames[7] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_1
    frames[8] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_1
    frames[9] = Rectangle(0, prev_y += 28, 24, 28); // skier_left_2
    frames[10] = Rectangle(0, prev_y += 28, 24, 28); // skier_right_2
    frames[11] = Rectangle(0, prev_y += 28, 32, 32); // skier_ouch
    frames[12] = Rectangle(0, prev_y += 32, 32, 24); // skier_sit
    frames[13] = Rectangle(0, prev_y += 24, 32, 32); // skier_jump_0
    frames[14] = Rectangle(0, prev_y += 32, 28, 31); // skier_trick_left_1
    frames[15] = Rectangle(0, prev_y += 31, 28, 31); // skier_trick_right_1
    frames[16] = Rectangle(0, prev_y += 31, 28, 34); // skier_trick_backwards
    frames[17] = Rectangle(0, prev_y += 34, 32, 26); // skier_hit
    frames[18] = Rectangle(0, prev_y += 26, 32, 32); // skier_jump_1
    frames[19] = Rectangle(0, prev_y += 32, 31, 24); // skier_jump_2
    frames[20] = Rectangle(0, prev_y += 24, 25, 31); // skier_trick_left_2
    frames[21] = Rectangle(0, prev_y += 31, 25, 31); // skier_trick_right_2
    frames[22] = Rectangle(0, prev_y += 31, 12, 24); // slalom_arrow_left
    frames[23] = Rectangle(0, prev_y += 24, 12, 24); // slalom_arrow_right
    frames[24] = Rectangle(0, prev_y += 24, 12, 24); // slalom_success
    frames[25] = Rectangle(0, prev_y += 24, 12, 24); // slalom_fail
    frames[26] = Rectangle(0, prev_y += 24, 64, 32); // mogul_group
    frames[27] = Rectangle(0, prev_y += 32, 24, 30); // novice_1
    frames[28] = Rectangle(0, prev_y += 30, 22, 29); // novice_2
    frames[29] = Rectangle(0, prev_y += 29, 21, 29); // novice_3
    frames[30] = Rectangle(0, prev_y += 29, 24, 24); // novice_sit
    frames[31] = Rectangle(0, prev_y += 24, 24, 24); // novice_ground
    frames[32] = Rectangle(0, prev_y += 24, 21, 15); // dog_1
    frames[33] = Rectangle(0, prev_y += 15, 21, 15); // dog_2
    frames[34] = Rectangle(0, prev_y += 15, 19, 19); // dog_hit_1
    frames[35] = Rectangle(0, prev_y += 19, 19, 19); // dog_hit_2
    frames[36] = Rectangle(0, prev_y += 19, 26, 30); // snowboarder_left
    frames[37] = Rectangle(0, prev_y += 30, 20, 30); // snowboarder_right
    frames[38] = Rectangle(0, prev_y += 30, 25, 31); // snowboarder_360_1
    frames[39] = Rectangle(0, prev_y += 31, 30, 29); // snowboarder_360_2
    frames[40] = Rectangle(0, prev_y += 29, 32, 32); // snowboarder_360_3
    frames[41] = Rectangle(0, prev_y += 32, 32, 32); // snowboarder_360_4
    frames[42] = Rectangle(0, prev_y += 32, 25, 29); // snowboarder_360_5
    frames[43] = Rectangle(0, prev_y += 29, 29, 25); // snowboarder_360_6
    frames[44] = Rectangle(0, prev_y += 25, 23, 11); // rock
    frames[45] = Rectangle(0, prev_y += 11, 16, 11); // stump
    frames[46] = Rectangle(0, prev_y += 11, 16, 4); // mogul_small
    frames[47] = Rectangle(0, prev_y += 4, 24, 8); // mogul_large
    frames[48] = Rectangle(0, prev_y += 8, 28, 32); // tree_small
    frames[49] = Rectangle(0, prev_y += 32, 22, 27); // tree_dried
    frames[50] = Rectangle(0, prev_y += 27, 32, 64); // tree_large
    frames[51] = Rectangle(0, prev_y += 64, 32, 8); // rainbow
    frames[52] = Rectangle(0, prev_y += 8, 93, 57); // title
    frames[53] = Rectangle(0, prev_y += 57, 52, 10); // version
    frames[54] = Rectangle(0, prev_y += 10, 92, 30); // numpad
    frames[55] = Rectangle(0, prev_y += 30, 63, 32); // restart_pause
    frames[56] = Rectangle(0, prev_y += 32, 42, 27); // start_left
    frames[57] = Rectangle(0, prev_y += 27, 42, 27); // start_right
    frames[58] = Rectangle(0, prev_y += 27, 50, 29); // finish_left
    frames[59] = Rectangle(0, prev_y += 29, 50, 29); // finish_right
    frames[60] = Rectangle(0, prev_y += 29, 40, 36); // slalom_sign
    frames[61] = Rectangle(0, prev_y += 36, 44, 36); // tree_slalom_sign
    frames[62] = Rectangle(0, prev_y += 36, 40, 35); // freestyle_sign
    frames[63] = Rectangle(0, prev_y += 35, 24, 64); // lift_pole
    frames[64] = Rectangle(0, prev_y += 64, 26, 32); // lift_chair_full
    frames[65] = Rectangle(0, prev_y += 32, 26, 32); // lift_chair_novice
    frames[66] = Rectangle(0, prev_y += 32, 26, 32); // lift_chair_empty
    frames[67] = Rectangle(0, prev_y += 32, 32, 48); // yeti_happy_1
    frames[68] = Rectangle(0, prev_y += 48, 32, 48); // yeti_happy_2
    frames[69] = Rectangle(0, prev_y += 48, 32, 48); // yeti_left_1
    frames[70] = Rectangle(0, prev_y += 48, 32, 48); // yeti_left_2
    frames[71] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_1
    frames[72] = Rectangle(0, prev_y += 48, 32, 48); // yeti_right_2
    frames[73] = Rectangle(0, prev_y += 48, 32, 48); // yeti_up_1
    frames[74] = Rectangle(0, prev_y += 48, 32, 48); // yeti_up_2
    frames[75] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_1
    frames[76] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_2
    frames[77] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_3
    frames[78] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_4
    frames[79] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_5
    frames[80] = Rectangle(0, prev_y += 48, 32, 48); // yeti_eating_6
    frames[81] = Rectangle(0, prev_y += 48, 16, 8); // urine
    frames[82] = Rectangle(0, prev_y += 8, 22, 27); // tree_fire_1
    frames[83] = Rectangle(0, prev_y += 27, 22, 27); // tree_fire_2
    frames[84] = Rectangle(0, prev_y += 27, 22, 27); // tree_fire_3
    frames[85] = Rectangle(0, prev_y += 27, 8, 11); // mushroom
    frames[86] = Rectangle(0, prev_y += 11, 28, 32); // tree_walk_idle
    frames[87] = Rectangle(0, prev_y += 32, 28, 32); // tree_walk_right
    frames[88] = Rectangle(0, prev_y + 32, 28, 32); // tree_walk_left

    camera.zoom = 1.0f;

    slalom_sign.type = SkiObject::TYPE_SLALOM_SIGN;
    slalom_sign.position = Vector2(-180, 270);
    slalom_sign.current_frame_index = 60;
    slalom_sign.current_frame_rectangle = frames[60];

    freestyle_sign.type = SkiObject::TYPE_FREESTYLE_SIGN;
    freestyle_sign.position = Vector2(0, 270);
    freestyle_sign.current_frame_index = 62;
    freestyle_sign.current_frame_rectangle = frames[62];

    tree_slalom_sign.type = SkiObject::TYPE_TREE_SLALOM_SIGN;
    tree_slalom_sign.position = Vector2(180, 270);
    tree_slalom_sign.current_frame_index = 61;
    tree_slalom_sign.current_frame_rectangle = frames[61];

    restart_pause_sign.type = SkiObject::TYPE_RESTART_PAUSE_SIGN;
    restart_pause_sign.position = Vector2(90, 0);
    restart_pause_sign.current_frame_index = 55;
    restart_pause_sign.current_frame_rectangle = frames[55];

    numpad_sign.type = SkiObject::TYPE_NUMPAD;
    numpad_sign.position = Vector2(90, -45);
    numpad_sign.current_frame_index = 54;
    numpad_sign.current_frame_rectangle = frames[54];

    start_left_freestyle_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_freestyle_sign.position = Vector2(-160, 800);
    start_left_freestyle_sign.current_frame_index = 56;
    start_left_freestyle_sign.current_frame_rectangle = frames[56];

    start_right_freestyle_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_freestyle_sign.position = Vector2(180, 800);
    start_right_freestyle_sign.current_frame_index = 57;
    start_right_freestyle_sign.current_frame_rectangle = frames[57];

    start_left_slalom_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_slalom_sign.position = Vector2(-540, 800);
    start_left_slalom_sign.current_frame_index = 56;
    start_left_slalom_sign.current_frame_rectangle = frames[56];

    start_right_slalom_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_slalom_sign.position = Vector2(-240, 800);
    start_right_slalom_sign.current_frame_index = 57;
    start_right_slalom_sign.current_frame_rectangle = frames[57];

    start_left_tree_slalom_sign.type = SkiObject::TYPE_START_LEFT;
    start_left_tree_slalom_sign.position = Vector2(260, 800);
    start_left_tree_slalom_sign.current_frame_index = 56;
    start_left_tree_slalom_sign.current_frame_rectangle = frames[56];

    start_right_tree_slalom_sign.type = SkiObject::TYPE_START_RIGHT;
    start_right_tree_slalom_sign.position = Vector2(560, 800);
    start_right_tree_slalom_sign.current_frame_index = 57;
    start_right_tree_slalom_sign.current_frame_rectangle = frames[57];

    finish_left_slalom_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_slalom_sign.position = Vector2(-540, 10800);
    finish_left_slalom_sign.current_frame_index = 58;
    finish_left_slalom_sign.current_frame_rectangle = frames[58];

    finish_right_slalom_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_slalom_sign.position = Vector2(-240, 10800);
    finish_right_slalom_sign.current_frame_index = 59;
    finish_right_slalom_sign.current_frame_rectangle = frames[59];

    finish_left_tree_slalom_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_tree_slalom_sign.position = Vector2(260, 20800);
    finish_left_tree_slalom_sign.current_frame_index = 58;
    finish_left_tree_slalom_sign.current_frame_rectangle = frames[58];

    finish_right_tree_slalom_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_tree_slalom_sign.position = Vector2(560, 20800);
    finish_right_tree_slalom_sign.current_frame_index = 59;
    finish_right_tree_slalom_sign.current_frame_rectangle = frames[59];

    finish_left_freestyle_sign.type = SkiObject::TYPE_FINISH_LEFT;
    finish_left_freestyle_sign.position = Vector2(-160, 20800);
    finish_left_freestyle_sign.current_frame_index = 58;
    finish_left_freestyle_sign.current_frame_rectangle = frames[58];

    finish_right_freestyle_sign.type = SkiObject::TYPE_FINISH_RIGHT;
    finish_right_freestyle_sign.position = Vector2(180, 20800);
    finish_right_freestyle_sign.current_frame_index = 59;
    finish_right_freestyle_sign.current_frame_rectangle = frames[59];

    long_live_objects.emplace_back(&slalom_sign);
    long_live_objects.emplace_back(&freestyle_sign);
    long_live_objects.emplace_back(&tree_slalom_sign);
    long_live_objects.emplace_back(&restart_pause_sign);
    long_live_objects.emplace_back(&numpad_sign);
    long_live_objects.emplace_back(&start_left_freestyle_sign);
    long_live_objects.emplace_back(&start_right_freestyle_sign);
    long_live_objects.emplace_back(&start_left_slalom_sign);
    long_live_objects.emplace_back(&start_right_slalom_sign);
    long_live_objects.emplace_back(&start_left_tree_slalom_sign);
    long_live_objects.emplace_back(&start_right_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_left_slalom_sign);
    long_live_objects.emplace_back(&finish_right_slalom_sign);
    long_live_objects.emplace_back(&finish_left_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_right_tree_slalom_sign);
    long_live_objects.emplace_back(&finish_left_freestyle_sign);
    long_live_objects.emplace_back(&finish_right_freestyle_sign);

    characters_objects.emplace_back(&player);
    characters_objects.emplace_back(&yeti_1);
    characters_objects.emplace_back(&yeti_2);

    for (size_t i = 0; i < 13; ++i)
    {
        lift_objects.emplace_back();
        auto *pole = &lift_objects.back();

        pole->type = SkiObject::TYPE_LIFT_POLE;
        pole->position.x = -90;
        int y = -65 * 20 + 128 * 20 * i;
        pole->position.y = y;
        pole->current_frame_index = 63;
        pole->current_frame_rectangle = frames[63];

        if (i < 12)
        {
            lift_objects.emplace_back();
            auto *chair_empty = &lift_objects.back();

            chair_empty->type = SkiObject::TYPE_LIFT_CHAIR;
            chair_empty->state = SkiObject::STATE_LIFT_CHAIR_EMPTY;
            chair_empty->position.x = -107;
            chair_empty->position.y = y;
            chair_empty->current_frame_index = 66;
            chair_empty->current_frame_rectangle = frames[66];
            chair_empty->speed = 3;
            chair_empty->direction.y = 1;
        }
        if (i > 0)
        {
            lift_objects.emplace_back();
            auto *chair_full = &lift_objects.back();

            chair_full->type = SkiObject::TYPE_LIFT_CHAIR;
            chair_full->state = SkiObject::STATE_LIFT_CHAIR_FULL;
            chair_full->position.x = -73;
            chair_full->position.y = y;
            chair_full->current_frame_index = 64;
            chair_full->current_frame_rectangle = frames[64];
            chair_full->speed = 3;
            chair_full->direction.y = -1;
        }
    }

    for (size_t i = 0; i < 24; ++i)
    {
        auto flag = SkiObject();
        if (i % 2 == 0)
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_LEFT;
            flag.state = SkiObject::STATE_SLALOM_LEFT;
            flag.position.x = -460;
            flag.position.y = 60 * 20 + 20 * 20 * i;
            flag.current_frame_index = 22;
            flag.current_frame_rectangle = frames[22];
        }
        else
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_RIGHT;
            flag.state = SkiObject::STATE_SLALOM_RIGHT;
            flag.position.x = -320;
            flag.position.y = 60 * 20 + 20 * 20 * i;
            flag.current_frame_index = 23;
            flag.current_frame_rectangle = frames[23];
        }
        slalom_flags_objects.push_back(flag);
    }

    for (size_t i = 0; i < 39; ++i)
    {
        auto flag = SkiObject();
        if (i % 2 == 0)
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_LEFT;
            flag.state = SkiObject::STATE_SLALOM_LEFT;
            flag.position.x = 340;
            flag.position.y = 65 * 20 + 25 * 20 * i;
            flag.current_frame_index = 22;
            flag.current_frame_rectangle = frames[22];
        }
        else
        {
            flag.type = SkiObject::TYPE_SLALOM_ARROW_RIGHT;
            flag.state = SkiObject::STATE_SLALOM_RIGHT;
            flag.position.x = 480;
            flag.position.y = 65 * 20 + 25 * 20 * i;
            flag.current_frame_index = 23;
            flag.current_frame_rectangle = frames[23];
        }
        slalom_flags_objects.push_back(flag);
    }

    reset();
};

SkiFree::~SkiFree()
{
    UnloadTexture(all_textures);
}

void SkiFree::draw() const
{
    BeginMode2D(camera);
    for (const auto &long_live_object: long_live_objects)
    {
        DrawTextureRec(
                all_textures, long_live_object->current_frame_rectangle,
                Vector2(long_live_object->position.x,
                        long_live_object->position.y + long_live_object->offset_y),
                WHITE);
    }
    for (const auto &character: characters_objects)
    {
        DrawTextureRec(
                all_textures, character->current_frame_rectangle,
                Vector2(character->position.x, character->position.y - character->offset_y), WHITE);
    }
    for (const auto &lift_obj: lift_objects)
    {
        DrawTextureRec(all_textures, lift_obj.current_frame_rectangle, lift_obj.position, WHITE);
    }
    for (const auto &slalom_flags: slalom_flags_objects)
    {
        DrawTextureRec(
                all_textures, slalom_flags.current_frame_rectangle, slalom_flags.position, WHITE);
    }
    for (const auto &short_live_obj: short_live_objects)
    {
        DrawTextureRec(
                all_textures, short_live_obj.current_frame_rectangle, short_live_obj.position,
                WHITE);
    }
    EndMode2D();

    DrawRectangle(0, 0, GetScreenWidth(), 30, BLUE);
    const char *text;
    if (is_paused)
    {
        text = "Ski Paused... Press F3 to continue";
    }
    else
    {
        text = "Ski Free";
    }
    const int width = MeasureText(text, 22);
    DrawText(text, (GetScreenWidth() - width) / 2, 5, 22, WHITE);

    DrawRectangleLinesEx(Rectangle(650, 30, 150, 76), 2, BLACK);

    auto elapsed = current_time - start_time;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(elapsed);
    elapsed -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsed);
    elapsed -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    elapsed -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

    DrawText(
            TextFormat(
                    "Time: %d:%d:%d.%d", hours.count(), minutes.count(), seconds.count(),
                    milliseconds.count()),
            655, 32, 14, BLACK);
    DrawText(TextFormat("Dist: %02dm", int(display_distance / 20)), 655, 48, 14, BLACK);
    DrawText(TextFormat("Speed: %.0fm/s", Vector2Length(player.velocity)), 655, 64, 14, BLACK);
    DrawText("Style: 0", 655, 80, 14, BLACK);
}

void SkiFree::inputs()
{
    if (IsKeyPressed(KEY_F2))
    {
        reset();
    }
    if (IsKeyPressed(KEY_F3))
    {
        is_paused = !is_paused;
        if (is_paused)
        {
            is_waiting_action = true;
        }
        else
        {
            is_waiting_action = false;
        }
    }
    if (is_paused && IsKeyPressed(KEY_T))
    {
        is_waiting_action = true;
    }
    if (is_paused && !is_waiting_action)
    {
        return;
    }
    if (player.state == SkiObject::STATE_PLAYER_EATING)
    {
        return;
    }

    if (IsKeyPressed(KEY_F))
    {
        if (speed_limit > 25)
        {
            speed_limit = 25;
        }
        else
        {
            speed_limit = 31;
        }
    }

    if (std::abs(player.offset_y) < 0.001 && player.state != SkiObject::STATE_PLAYER_OUCH &&
        player.state != SkiObject::STATE_PLAYER_HIT)
    {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_KP_6))
        {
            is_waiting_action = false;
            switch (player.state)
            {
                case SkiObject::STATE_PLAYER_SIT:
                {
                    player.state = SkiObject::STATE_PLAYER_RIGHT;
                    player.current_frame_index = 6;
                    player.direction = {1, 0};
                    break;
                }
                case SkiObject::STATE_PLAYER_RIGHT:
                {
                    // 6, 8, 10
                    if (player.current_frame_index == 6)
                    {
                        player.current_frame_index = 8;
                    }
                    else if (player.current_frame_index == 8)
                    {
                        player.current_frame_index = 10;
                    }
                    else if (player.current_frame_index == 10)
                    {
                        player.current_frame_index = 6;
                    }
                    player.direction = {1, 0};
                    player.speed = 10;
                    break;
                }
                case SkiObject::STATE_PLAYER_LEFT:
                {
                    player.state = SkiObject::STATE_PLAYER_60_LEFT;
                    player.current_frame_index = 2;
                    player.direction.x = cosf(150 * DEG2RAD);
                    player.direction.y = sinf(150 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_60_LEFT:
                {
                    player.state = SkiObject::STATE_PLAYER_30_LEFT;
                    player.current_frame_index = 1;
                    player.direction.x = cosf(120 * DEG2RAD);
                    player.direction.y = sinf(120 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_30_LEFT:
                {
                    player.state = SkiObject::STATE_PLAYER_DOWN;
                    player.current_frame_index = 0;
                    player.direction.x = cosf(90 * DEG2RAD);
                    player.direction.y = sinf(90 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_DOWN:
                {
                    player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                    player.current_frame_index = 4;
                    player.direction.x = cosf(60 * DEG2RAD);
                    player.direction.y = sinf(60 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_30_RIGHT:
                {
                    player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                    player.current_frame_index = 5;
                    player.direction.x = cosf(30 * DEG2RAD);
                    player.direction.y = sinf(30 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_60_RIGHT:
                {
                    player.state = SkiObject::STATE_PLAYER_RIGHT;
                    player.current_frame_index = 6;
                    player.direction = {1, 0};
                    break;
                }
                default:
                    break;
            }
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_KP_4))
        {
            is_waiting_action = false;
            switch (player.state)
            {
                case SkiObject::STATE_PLAYER_SIT:
                {
                    player.state = SkiObject::STATE_PLAYER_LEFT;
                    player.current_frame_index = 3;
                    player.direction = {-1, 0};
                    break;
                }
                case SkiObject::STATE_PLAYER_LEFT:
                {
                    // 3, 7, 9
                    if (player.current_frame_index == 3)
                    {
                        player.current_frame_index = 7;
                    }
                    else if (player.current_frame_index == 7)
                    {
                        player.current_frame_index = 9;
                    }
                    else if (player.current_frame_index == 9)
                    {
                        player.current_frame_index = 3;
                    }
                    player.direction = {-1, 0};
                    player.speed = 10;
                    break;
                }
                case SkiObject::STATE_PLAYER_RIGHT:
                {
                    player.state = SkiObject::STATE_PLAYER_60_RIGHT;
                    player.current_frame_index = 5;
                    player.direction.x = cosf(30 * DEG2RAD);
                    player.direction.y = sinf(30 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_60_RIGHT:
                {
                    player.state = SkiObject::STATE_PLAYER_30_RIGHT;
                    player.current_frame_index = 4;
                    player.direction.x = cosf(60 * DEG2RAD);
                    player.direction.y = sinf(60 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_30_RIGHT:
                {
                    player.state = SkiObject::STATE_PLAYER_DOWN;
                    player.current_frame_index = 0;
                    player.direction = {0, 1};
                    break;
                }
                case SkiObject::STATE_PLAYER_DOWN:
                {
                    player.state = SkiObject::STATE_PLAYER_30_LEFT;
                    player.current_frame_index = 1;
                    player.direction.x = cosf(120 * DEG2RAD);
                    player.direction.y = sinf(120 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_30_LEFT:
                {
                    player.state = SkiObject::STATE_PLAYER_60_LEFT;
                    player.current_frame_index = 2;
                    player.direction.x = cosf(150 * DEG2RAD);
                    player.direction.y = sinf(150 * DEG2RAD);
                    break;
                }
                case SkiObject::STATE_PLAYER_60_LEFT:
                {
                    player.state = SkiObject::STATE_PLAYER_LEFT;
                    player.current_frame_index = 3;
                    player.direction = {-1, 0};
                    break;
                }
                default:
                    break;
            }
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_KP_9))
        {
            is_waiting_action = false;
            player.state = SkiObject::STATE_PLAYER_RIGHT;
            player.current_frame_index = 6;
            player.direction = {1, 0};
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_KP_7))
        {
            is_waiting_action = false;
            player.state = SkiObject::STATE_PLAYER_LEFT;
            player.current_frame_index = 3;
            player.direction = {-1, 0};
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_KP_3))
        {
            is_waiting_action = false;
            player.state = SkiObject::STATE_PLAYER_30_RIGHT;
            player.current_frame_index = 4;
            player.direction.x = cosf(60 * DEG2RAD);
            player.direction.y = sinf(60 * DEG2RAD);
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_KP_1))
        {
            is_waiting_action = false;
            player.state = SkiObject::STATE_PLAYER_30_LEFT;
            player.current_frame_index = 1;
            player.direction.x = cosf(120 * DEG2RAD);
            player.direction.y = sinf(120 * DEG2RAD);
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_KP_2))
        {
            is_waiting_action = false;
            player.state = SkiObject::STATE_PLAYER_DOWN;
            player.current_frame_index = 0;
            player.direction = {0, 1};
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_KP_8))
        {
            is_waiting_action = false;
            switch (player.state)
            {
                case SkiObject::STATE_PLAYER_SIT:
                {
                    player.state = SkiObject::STATE_PLAYER_RIGHT;
                    player.current_frame_index = 6;
                    player.direction = {1, 0};
                    break;
                }
                case SkiObject::STATE_PLAYER_LEFT:
                {
                    if (player.current_frame_index == 3)
                    {
                        player.current_frame_index = 7;
                    }
                    else if (player.current_frame_index == 7)
                    {
                        player.current_frame_index = 9;
                    }
                    else if (player.current_frame_index == 9)
                    {
                        player.current_frame_index = 3;
                    }
                    player.direction = {0, -1};
                    player.speed = 10;
                    break;
                }
                case SkiObject::STATE_PLAYER_RIGHT:
                {
                    // 6, 8, 10
                    if (player.current_frame_index == 6)
                    {
                        player.current_frame_index = 8;
                    }
                    else if (player.current_frame_index == 8)
                    {
                        player.current_frame_index = 10;
                    }
                    else if (player.current_frame_index == 10)
                    {
                        player.current_frame_index = 6;
                    }
                    player.direction = {0, -1};
                    player.speed = 10;
                    break;
                }
                default:
                    break;
            }
            player.current_frame_rectangle = frames[player.current_frame_index];
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            is_waiting_action = false;
            player_jump(5);
        }
    }
    else if (IsKeyPressed(KEY_SPACE) && player.current_frame_index == 13)
    {
        is_waiting_action = false;
        player.current_frame_index = 18;
        player.current_frame_rectangle = frames[18];
    }
    else if (IsKeyPressed(KEY_SPACE) && player.current_frame_index == 18)
    {
        is_waiting_action = false;
        player.current_frame_index = 19;
        player.current_frame_rectangle = frames[19];
    }
    else if (
            (IsKeyPressed(KEY_SPACE) && player.current_frame_index == 19) ||
            (IsKeyPressed(KEY_RIGHT) &&
             (player.current_frame_index == 14 || player.current_frame_index == 20)) ||
            (IsKeyPressed(KEY_LEFT) &&
             (player.current_frame_index == 15 || player.current_frame_index == 21)))
    {
        is_waiting_action = false;
        player.current_frame_index = 13;
        player.current_frame_rectangle = frames[13];
    }
    else if (
            (IsKeyPressed(KEY_LEFT) && player.current_frame_index == 13) ||
            (IsKeyPressed(KEY_RIGHT) && player.current_frame_index == 16))
    {
        is_waiting_action = false;
        player.current_frame_index = 14;
        player.current_frame_rectangle = frames[14];
    }
    else if (
            (IsKeyPressed(KEY_RIGHT) && player.current_frame_index == 13) ||
            (IsKeyPressed(KEY_LEFT) && player.current_frame_index == 16))
    {
        is_waiting_action = false;
        player.current_frame_index = 15;
        player.current_frame_rectangle = frames[15];
    }
    else if (
            (IsKeyPressed(KEY_LEFT) &&
             (player.current_frame_index == 14 || player.current_frame_index == 20)) ||
            (IsKeyPressed(KEY_RIGHT) &&
             (player.current_frame_index == 15 || player.current_frame_index == 21)))
    {
        is_waiting_action = false;
        player.current_frame_index = 16;
        player.current_frame_rectangle = frames[16];
    }
    else if (
            (IsKeyPressed(KEY_LEFT) && player.current_frame_index == 18) ||
            (IsKeyPressed(KEY_SPACE) && player.current_frame_index == 14))
    {
        is_waiting_action = false;
        player.current_frame_index = 20;
        player.current_frame_rectangle = frames[20];
    }
    else if (
            (IsKeyPressed(KEY_RIGHT) && player.current_frame_index == 18) ||
            (IsKeyPressed(KEY_SPACE) && player.current_frame_index == 15))
    {
        is_waiting_action = false;
        player.current_frame_index = 21;
        player.current_frame_rectangle = frames[21];
    }
}

void SkiFree::update()
{
    if (is_paused && !IsKeyPressed(KEY_T))
    {
        return;
    }
    Vector2 pos_before = player.position;
    for (const auto character: characters_objects)
    {
        character->update(frames, speed_limit);
    }

    // teleport yeti_1 based on player position thresholds
    if (player_crossed_down(2000, pos_before))
    {
        yeti_1.position.y = 2100 * 20;
    }
    if (player_crossed_up(0, pos_before))
    {
        yeti_1.position.y = -130 * 20;
    }

    // teleport objects at 2050 for game continuity
    if (player_crossed_down(2050, pos_before))
    {
        // teleport objects up mountain
        for (auto &obj: short_live_objects)
        {
            obj.position.y -= 2050 * 20 * 2;
        }
        for (auto &obj: characters_objects)
        {
            obj->position.y -= 2050 * 20 * 2;
        }
    }
    if (player_crossed_up(2050, pos_before))
    {
        // teleport objects down mountain
        for (auto &obj: short_live_objects)
        {
            obj.position.y += 2050 * 20 * 2;
        }
        for (auto &obj: characters_objects)
        {
            obj->position.y += 2050 * 20 * 2;
        }
    }

    if (player.position.y > -2050 * 20 && player.position.y < -125 * 20 &&
        yeti_1.state != SkiObject::STATE_YETI_EATING &&
        player.state != SkiObject::STATE_PLAYER_EATING)
    {
        yeti_chase(&yeti_1, 29.0f);
    }
    else if (yeti_1.speed > 0 && yeti_1.state != SkiObject::STATE_YETI_EATING)
    {
        yeti_stop(&yeti_1);
    }

    if (player.position.y > 2000 * 20 && player.position.y < 2050 * 20 &&
        yeti_2.state != SkiObject::STATE_YETI_EATING &&
        player.state != SkiObject::STATE_PLAYER_EATING)
    {
        yeti_chase(&yeti_2, 27.0f);
    }
    else if (
            yeti_2.speed > 0 && yeti_1.state != SkiObject::STATE_YETI_EATING &&
            player.state != SkiObject::STATE_PLAYER_EATING)
    {
        yeti_stop(&yeti_2);
    }

    if (player_crossed_down(40, pos_before))
    {
        if (player.position.x >= -540 && player.position.x <= -240 + 27)
        {
            start_slalom();
        }
        if (player.position.x >= -160 && player.position.x <= 180 + 27)
        {
            start_freestyle();
        }
        if (player.position.x >= 260 && player.position.x <= 560 + 27)
        {
            start_tree_slalom();
        }
    }

    if (player_crossed_up(40, pos_before))
    {
        current_mode = MODE_PRACTICE;
    }

    if (current_mode == MODE_SLALOM || current_mode == MODE_TREE_SLALOM)
    {
        current_time = std::chrono::steady_clock::now();
    }

    if (current_mode == MODE_PRACTICE)
    {
        display_distance = player.position.y;
    }
    else
    {
        display_distance = end_distance - player.position.y;
    }

    manage_objects();
    for (const auto long_live_object: long_live_objects)
    {
        long_live_object->update(frames, speed_limit);
    }
    for (auto &lift_obj: lift_objects)
    {
        lift_obj.update(frames, speed_limit);
        if (lift_obj.state == SkiObject::STATE_LIFT_CHAIR_FULL)
        {
            if (lift_obj.state_countdown == 0 &&
                std::abs(lift_obj.position.y - player.position.y) < 128)
            {
                lift_obj.state_countdown = GetRandomValue(100, 256);
            }
            if (lift_obj.state_countdown == 1)
            {
                auto new_snowboarder = create_snowboarder();
                new_snowboarder.position = lift_obj.position;
                short_live_objects.push_back(new_snowboarder);
                lift_obj.state = SkiObject::STATE_LIFT_CHAIR_NOVICE;
                lift_obj.current_frame_index = 65;
                lift_obj.current_frame_rectangle = frames[65];
            }
        }
    }
    for (auto &short_live_object: short_live_objects)
    {
        short_live_object.update(frames, speed_limit);
    }

    collisions_manager();

    if (current_mode == MODE_SLALOM)
    {
        slalom_manager(current_mode, pos_before);
    }

    camera.offset = GetWorldToScreen2D(player.position, camera);
    camera.target = player.position;
    Vector2 delta = Vector2(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0 - 150) -
                    GetWorldToScreen2D(player.position, camera);
    delta *= -1.0f / camera.zoom;
    camera.target += delta;
}

SkiObject SkiFree::create_snowboarder()
{
    auto snowboarder = SkiObject();
    snowboarder.type = SkiObject::TYPE_SNOWBOARDER;
    int dir = GetRandomValue(0, 1) * 2 - 1;
    snowboarder.direction.x = dir;
    if (dir < 0)
    {
        snowboarder.state = SkiObject::STATE_SNOWBOARDER_LEFT;
        snowboarder.current_frame_index = 36;
    }
    else
    {
        snowboarder.state = SkiObject::STATE_SNOWBOARDER_RIGHT;
        snowboarder.current_frame_index = 37;
    }
    snowboarder.direction.y = 1;
    snowboarder.state_countdown = 10;
    snowboarder.speed = 10;
    snowboarder.current_frame_rectangle = frames[snowboarder.current_frame_index];

    return snowboarder;
}

void SkiFree::manage_objects()
{
    auto new_area = Rectangle(
            player.position.x - area_size / 2, player.position.y - area_size / 2, area_size,
            area_size);

    if (abs(new_area.x - current_area.x) < 250 && abs(new_area.y - current_area.y) < 250)
    {
        return;
    }

    short_live_objects.remove_if([&new_area](const SkiObject &object)
                                 { return !CheckCollisionPointRec(object.position, new_area); });

    auto get_new_position = [&new_area]()
    {
        return Vector2(
                GetRandomValue(new_area.x, new_area.x + new_area.width),
                GetRandomValue(new_area.y, new_area.y + new_area.height));
    };

    auto check_collision_skiobjects = [&](SkiObject ski_object)
    {
        Rectangle ski_obj_loc = ski_object.get_location();
        for (const auto *ll: long_live_objects)
        {
            if (CheckCollisionRecs(ski_obj_loc, ll->get_location()))
            {
                return true;
            }
        }
        for (const auto &sl: short_live_objects)
        {
            if (CheckCollisionRecs(ski_obj_loc, sl.get_location()))
            {
                return true;
            }
        }
        return false;
    };

    auto check_slalom_area = [](SkiObject ski_object)
    {
        // Slalom area allows only TYPE_MOGUL_GROUP
        if (ski_object.type == SkiObject::TYPE_MOGUL_GROUP)
        {
            return false;
        }
        Rectangle ski_obj_loc = ski_object.get_location();
        static Rectangle slalom_area = Rectangle(-540, 800, 300, 10080);
        if (CheckCollisionRecs(ski_obj_loc, slalom_area))
        {
            return true;
        }
        return false;
    };

    auto check_tree_slalom_area = [](SkiObject ski_object)
    {
        // Tree-Slalom area allows only tree types
        if (ski_object.type == SkiObject::TYPE_TREE_DRIED ||
            ski_object.type == SkiObject::TYPE_TREE_LARGE ||
            ski_object.type == SkiObject::TYPE_TREE_SMALL ||
            ski_object.type == SkiObject::TYPE_TREE_WALK)
        {
            return false;
        }
        Rectangle ski_obj_loc = ski_object.get_location();
        static Rectangle slalom_area = Rectangle(260, 800, 300, 20080);
        if (CheckCollisionRecs(ski_obj_loc, slalom_area))
        {
            return true;
        }
        return false;
    };

    const size_t current_num_objs = short_live_objects.size();

    for (size_t i = 0; i < num_elements_in_area - current_num_objs; i++)
    {
        auto new_object = SkiObject();
        const auto type = (SkiObject::object_type) GetRandomValue(
                SkiObject::TYPE_MOGUL_GROUP, SkiObject::TYPE_RAINBOW);
        new_object.type = type;
        switch (type)
        {
            case SkiObject::TYPE_MOGUL_GROUP:
            {
                new_object.current_frame_index = 26;
                break;
            }
            case SkiObject::TYPE_ROCK:
            {
                new_object.current_frame_index = 44;
                break;
            }
            case SkiObject::TYPE_STUMP:
            {
                new_object.current_frame_index = 45;
                break;
            }
            case SkiObject::TYPE_MOGUL_SMALL:
            {
                new_object.current_frame_index = 46;
                break;
            }
            case SkiObject::TYPE_MOGUL_LARGE:
            {
                new_object.current_frame_index = 47;
                break;
            }
            case SkiObject::TYPE_TREE_SMALL:
            {
                new_object.current_frame_index = 48;
                break;
            }
            case SkiObject::TYPE_TREE_DRIED:
            {
                new_object.current_frame_index = 49;
                new_object.state = SkiObject::STATE_TREE_DRIED_NORMAL;
                break;
            }
            case SkiObject::TYPE_TREE_LARGE:
            {
                new_object.current_frame_index = 50;
                break;
            }
            case SkiObject::TYPE_RAINBOW:
            {
                new_object.current_frame_index = 51;
                break;
            }
            case SkiObject::TYPE_NOVICE:
            {
                new_object.current_frame_index = 27;
                int dir = GetRandomValue(0, 1);
                new_object.direction.x = cosf((60 + 60 * dir) * DEG2RAD);
                new_object.direction.y = sinf((60 + 60 * dir) * DEG2RAD);
                new_object.state = SkiObject::STATE_NOVICE_NORMAL;
                new_object.state_countdown = 4;
                new_object.speed = 2;
                break;
            }
            case SkiObject::TYPE_DOG:
            {
                new_object.current_frame_index = 32;
                new_object.direction.x = 1;
                new_object.direction.y = -1;
                new_object.state = SkiObject::STATE_DOG_RIGHT;
                new_object.state_countdown = 2;
                new_object.speed = 2;
                break;
            }
            case SkiObject::TYPE_SNOWBOARDER:
            {
                new_object = create_snowboarder();
                break;
            }
            case SkiObject::TYPE_TREE_WALK:
            {
                new_object.current_frame_index = 86;
                int dir = GetRandomValue(0, 1);
                new_object.direction.x = dir * 2 - 1;
                new_object.direction.y = 0;
                new_object.state = SkiObject::object_state(SkiObject::STATE_TREE_WALK_LEFT + dir);
                new_object.state_countdown = 36;
                new_object.speed = 2;
                break;
            }
            default:
            {
                (void) fprintf(stderr, "Invalid object type %d\n", type);
                assert(0);
            }
        }
        new_object.current_frame_rectangle = frames[new_object.current_frame_index];
        do
        {
            new_object.position = get_new_position();
        }
        while (CheckCollisionPointRec(new_object.position, current_area) ||
               check_collision_skiobjects(new_object) || check_slalom_area(new_object) ||
               check_tree_slalom_area(new_object));
        short_live_objects.push_back(new_object);
    }

    current_area = new_area;
}

void SkiFree::reset()
{
    SetRandomSeed(0);
    player.type = SkiObject::TYPE_SKIER;
    player.position = Vector2(0, 0);
    player.current_frame_index = 3;
    player.current_frame_rectangle = frames[player.current_frame_index];
    player.speed = 0;
    player.direction = {1, 0};
    player.state = SkiObject::STATE_PLAYER_LEFT;

    current_area = Rectangle(player.position.x - 100, player.position.y - 100, 100, 100);

    short_live_objects.clear();

    is_paused = false;
    is_waiting_action = true;

    yeti_1.type = SkiObject::TYPE_YETI;
    yeti_1.state = SkiObject::STATE_YETI_HAPPY_1;
    yeti_1.position = Vector2(0, -130 * 20);
    yeti_1.current_frame_index = 67;
    yeti_1.current_frame_rectangle = frames[67];
    yeti_1.state_countdown = GetRandomValue(1, 25);

    yeti_2.type = SkiObject::TYPE_YETI;
    yeti_2.state = SkiObject::STATE_YETI_HAPPY_1;
    yeti_2.position = Vector2(100, 2000 * 20);
    yeti_2.current_frame_index = 67;
    yeti_2.current_frame_rectangle = frames[67];
    yeti_2.state_countdown = GetRandomValue(1, 25);

    current_mode = MODE_PRACTICE;
    start_time = std::chrono::steady_clock::now();
    current_time = start_time;

    manage_objects();
}

void SkiFree::collisions_manager()
{
    if (player.state == SkiObject::STATE_PLAYER_EATING)
    {
        return;
    }

    auto p_box = player.get_collision_box();
    auto p_box_large = p_box;
    p_box_large.min.z = player.position.y;
    for (auto &character: characters_objects)
    {
        if (character->type == SkiObject::TYPE_SKIER)
        {
            continue;
        }
        auto c_box = character->get_collision_box();

        if (CheckCollisionBoxes(p_box_large, c_box))
        {
            if (character->type == SkiObject::TYPE_YETI)
            {
                character->state = SkiObject::STATE_YETI_EATING;
                character->current_frame_index = 75;
                character->current_frame_rectangle = frames[75];
                character->state_countdown = 6;
                character->speed = 0;
                character->offset_y = 0;

                player.current_frame_rectangle = {};
                player.state = SkiObject::STATE_PLAYER_EATING;
                player.speed = 0;
                return;
            }
        }
    }

    for (auto &lift_obj: lift_objects)
    {
        auto c_box = lift_obj.get_collision_box();
        if (CheckCollisionBoxes(p_box, c_box))
        {
            player_hit(c_box);

            return;
        }
    }

    for (auto &obj: short_live_objects)
    {
        auto c_box = obj.get_collision_box();
        if (c_box.max.x == 0)
        {
            continue;
        }
        if (CheckCollisionBoxes(p_box, c_box))
        {

            if (obj.type == SkiObject::TYPE_DOG && obj.state != SkiObject::STATE_DOG_HIT &&
                player.speed != 0)
            {
                obj.state = SkiObject::STATE_DOG_HIT;
                obj.current_frame_index = 34;
                obj.current_frame_rectangle = frames[34];
                obj.state_countdown = 20;
                obj.speed = 0;

                if (player.speed > 10)
                {
                    auto urine = SkiObject();
                    urine.type = SkiObject::TYPE_URINE;
                    urine.current_frame_index = 81;
                    urine.current_frame_rectangle = frames[81];
                    urine.position.x = obj.position.x;
                    urine.position.y = obj.position.y + 8;
                    short_live_objects.push_back(urine);
                }

                player.speed *= 0.6;
                return;
            }
            else if (obj.type == SkiObject::TYPE_URINE)
            {
                player.speed *= 0.6;
                player.speed = std::max(player.speed, 2.0);
                return;
            }
            else if (obj.type == SkiObject::TYPE_MOGUL_GROUP && player.direction.y > 0)
            {
                player.speed *= 0.6;
                player_jump(2);
                return;
            }
            else if (obj.type == SkiObject::TYPE_NOVICE)
            {
                obj.state = SkiObject::STATE_NOVICE_SIT;
                obj.current_frame_index = 30;
                obj.current_frame_rectangle = frames[30];
                obj.speed = 0;

                player_hit(c_box);
                return;
            }
            else if (
                    obj.type == SkiObject::TYPE_SNOWBOARDER ||
                    obj.type == SkiObject::TYPE_TREE_WALK ||
                    obj.type == SkiObject::TYPE_TREE_SMALL ||
                    obj.type == SkiObject::TYPE_TREE_LARGE)
            {
                player_hit(c_box);
                return;
            }
            else if (obj.type == SkiObject::TYPE_ROCK)
            {
                if (player.offset_y > 0)
                {
                    player_jump(2);
                    return;
                }

                player_hit(c_box);
                return;
            }
            else if (obj.type == SkiObject::TYPE_STUMP)
            {
                if (player.offset_y > 0)
                {
                    player_jump(2);
                    return;
                }

                if (player.direction.y < 0)
                {
                    obj.current_frame_index = 85;
                    obj.current_frame_rectangle = frames[85];
                }

                player_hit(c_box);
                return;
            }
            else if (obj.type == SkiObject::TYPE_TREE_DRIED)
            {
                player_hit(c_box);

                if (player.offset_y > 0 && player.speed > 10)
                {
                    obj.state = SkiObject::STATE_TREE_DRIED_FIRE;
                    obj.current_frame_index = 82;
                    obj.current_frame_rectangle = frames[82];
                }
                return;
            }
            else if (obj.type == SkiObject::TYPE_MOGUL_SMALL)
            {
                player_jump(2);
                return;
            }
            else if (obj.type == SkiObject::TYPE_MOGUL_LARGE)
            {
                player_jump(2);
                return;
            }
            else if (obj.type == SkiObject::TYPE_RAINBOW && player.direction.y > 0)
            {
                int jump = Clamp(player.speed, 2.0, 10.0);
                player_jump(jump);
                return;
            }
        }

        if (obj.type == SkiObject::TYPE_NOVICE)
        {
            for (auto &other: short_live_objects)
            {
                if (&other == &obj)
                {
                    continue;
                }

                switch (other.type)
                {
                    case SkiObject::TYPE_DOG:
                    case SkiObject::TYPE_RAINBOW:
                    {
                        break;
                    }
                    default:
                    {
                        if (CheckCollisionRecs(obj.get_location(), other.get_location()))
                        {
                            obj.state = SkiObject::STATE_NOVICE_SIT;
                            obj.current_frame_index = 30;
                            obj.current_frame_rectangle = frames[30];
                            obj.speed = 0;
                        }
                        break;
                    }
                }
            }
        }
    }
}

void SkiFree::player_hit(const BoundingBox &other_box)
{
    if (player.direction.y < 0)
    {
        player.position.y -= ((other_box.max.z - other_box.min.z) + 5);
    }
    else
    {
        player.position.y = other_box.max.z + 1;
    }
    player.state = SkiObject::STATE_PLAYER_HIT;
    player.current_frame_index = 17;
    player.current_frame_rectangle = frames[17];
}

void SkiFree::player_jump(const int jump_height)
{
    player.state = SkiObject::STATE_PLAYER_JUMP;
    player.current_frame_index = 13;
    player.current_frame_rectangle = frames[13];
    player.jump += jump_height + 0.5 * player.speed;
    player.speed = std::max(player.speed, 2.0);
    player.direction = Vector2(0, 1);
    player.offset_y += 1;
    if (player.offset_y > 0)
    {
        player.speed += 2;
    }
}

void SkiFree::yeti_chase(SkiObject *yeti, float speed)
{
    auto distance = player.position - yeti->position;
    if (std::abs(distance.x) > GetScreenWidth() * 2)
    {
        yeti->position.x =
                player.position.x - GetScreenWidth() * 2 * distance.x / std::abs(distance.x);
    }
    if (std::abs(distance.y) > GetScreenHeight() * 20 * 2)
    {
        yeti->position.y =
                player.position.y - GetScreenHeight() * 20 * 2 * distance.y / std::abs(distance.y);
    }

    yeti->direction = Vector2Normalize(player.position - yeti->position);
    if (yeti->direction.y < 0)
    {
        yeti->state = SkiObject::STATE_YETI_UP;
    }
    else if (yeti->direction.x > 0)
    {
        yeti->state = SkiObject::STATE_YETI_RIGHT;
    }
    else
    {
        yeti->state = SkiObject::STATE_YETI_LEFT;
    }
    yeti->speed = speed;
    yeti->offset_y = 0;
}

void SkiFree::yeti_stop(SkiObject *yeti)
{
    yeti->speed = 0;
    yeti->offset_y = 0;
    yeti->state = SkiObject::STATE_YETI_HAPPY_1;
    yeti->state_countdown = GetRandomValue(1, 25);
}

void SkiFree::start_slalom()
{
    current_mode = MODE_SLALOM;
    start_time = std::chrono::steady_clock::now();
    end_distance = 540 * 20;
}

void SkiFree::start_freestyle()
{
    current_mode = MODE_FREESTYLE;
    end_distance = 1040 * 20;
}

void SkiFree::start_tree_slalom()
{
    current_mode = MODE_TREE_SLALOM;
    start_time = std::chrono::steady_clock::now();
    end_distance = 1040 * 20;
}

bool SkiFree::player_crossed_down(const float threshold, const Vector2 pos_before)
{
    return player.position.y >= threshold * 20 && pos_before.y < threshold * 20;
}

bool SkiFree::player_crossed_up(const float threshold, const Vector2 pos_before)
{
    return player.position.y < threshold * 20 && pos_before.y >= threshold * 20;
}

void SkiFree::slalom_manager(const game_mode_t current_mode, const Vector2 pos_before)
{
    for (auto &flag: slalom_flags_objects)
    {
        if (player_crossed_down(flag.position.y / 20, pos_before))
        {
            if ((current_mode == MODE_SLALOM && flag.position.x < 0) ||
                (current_mode == MODE_TREE_SLALOM && flag.position.x > 0))
            {
                auto distance = flag.position - player.position;
                if (flag.state == SkiObject::STATE_SLALOM_LEFT)
                {
                    if (distance.x > 0)
                    {
                        flag.state = SkiObject::STATE_SLALOM_SUCCESS;
                        flag.current_frame_index = 24;
                        flag.current_frame_rectangle = frames[24];
                    }
                    else
                    {
                        flag.state = SkiObject::STATE_SLALOM_FAIL;
                        flag.current_frame_index = 25;
                        flag.current_frame_rectangle = frames[25];
                    }
                }

                if (flag.state == SkiObject::STATE_SLALOM_RIGHT)
                {
                    if (distance.x < 0)
                    {
                        flag.state = SkiObject::STATE_SLALOM_SUCCESS;
                        flag.current_frame_index = 24;
                        flag.current_frame_rectangle = frames[24];
                    }
                    else
                    {
                        flag.state = SkiObject::STATE_SLALOM_FAIL;
                        flag.current_frame_index = 25;
                        flag.current_frame_rectangle = frames[25];
                    }
                }
            }
        }
    }
}
