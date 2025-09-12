#include <algorithm>
#include <cstdlib>
#include "SnakeGameAI.h"

#include <cassert>
#include <cstring>
#include <list>


inline bool operator==(const Vector2i &lhs, const Vector2i &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline Vector2i operator+(const Vector2i &lhs, const Vector2i &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vector2i operator-(const Vector2i &lhs, const Vector2i &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

SnakeGameAI::SnakeGameAI()
{
    SnakeGameAI::reset();
};

void SnakeGameAI::new_food()
{
    // NOLINTNEXTLINE
    const int x = rand() % w;
    // NOLINTNEXTLINE
    const int y = rand() % h;
    food = Vector2i(x, y);
}

void SnakeGameAI::place_food()
{
    new_food();
    while (std::ranges::find(snake.begin(), snake.end(), food) != snake.end())
    {
        new_food();
    }
};

bool SnakeGameAI::is_collision(const Vector2i pt) const
{
    // hits boundary
    if (pt.x > w - 1 || pt.x < 0 || pt.y > h - 1 || pt.y < 0)
    {
        return true;
    }
    // hits itself
    if (auto it = std::find(snake.begin() + 1, snake.end(), pt); it != snake.end())
    {
        return true;
    }
    return false;
}

size_t SnakeGameAI::index_from_location(Vector2i pt) const
{
    return pt.y * w + pt.x;
}

void SnakeGameAI::update_field()
{
    memset(distance_field, -1, sizeof(int) * w * h);
    std::list<FieldLoc> nodes;
    nodes.emplace_back(food, 1);
    while (!nodes.empty())
    {
        std::list<FieldLoc> new_nodes;
        for (auto &[pt, distance]: nodes)
        {
            distance_field[index_from_location(pt)] = distance;
            Vector2i right = pt + p_right;
            Vector2i left = pt + p_left;
            Vector2i up = pt + p_up;
            Vector2i down = pt + p_down;
            if (!is_collision(right) && distance_field[index_from_location(right)] == 0)
            {
                new_nodes.emplace_back(right, distance + 1);
            }
            if (!is_collision(left) && distance_field[index_from_location(left)] == 0)
            {
                new_nodes.emplace_back(left, distance + 1);
            }
            if (!is_collision(up) && distance_field[index_from_location(up)] == 0)
            {
                new_nodes.emplace_back(up, distance + 1);
            }
            if (!is_collision(down) && distance_field[index_from_location(down)] == 0)
            {
                new_nodes.emplace_back(down, distance + 1);
            }
        }

        new_nodes.sort([&](const FieldLoc &a, const FieldLoc &b)
                       { return index_from_location(a.p) < index_from_location(b.p); });
        new_nodes.unique([&](const FieldLoc &a, const FieldLoc &b) { return a.p == b.p; });
        nodes.clear();
        nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());
    }
}

std::vector<float> SnakeGameAI::get_state() const
{
    auto [head_x, head_y] = snake[0];

    // get points around the head
    const auto point_left = Vector2i(head_x - 1, head_y);
    const auto point_right = Vector2i(head_x + 1, head_y);
    const auto point_up = Vector2i(head_x, head_y - 1);
    const auto point_down = Vector2i(head_x, head_y + 1);

    const bool is_direction_right = curr_action == ACTION_RIGHT;
    const bool is_direction_left = curr_action == ACTION_LEFT;
    const bool is_direction_up = curr_action == ACTION_UP;
    const bool is_direction_down = curr_action == ACTION_DOWN;

    const bool is_food_left = food.x < head_x;
    const bool is_food_right = food.x > head_x;
    const bool is_food_up = food.y < head_y;
    const bool is_food_down = food.y > head_y;

    const bool right_reaches_food = distance_field[index_from_location(point_right)] > 0;
    const bool left_reaches_food = distance_field[index_from_location(point_left)] > 0;
    const bool up_reaches_food = distance_field[index_from_location(point_up)] > 0;
    const bool down_reaches_food = distance_field[index_from_location(point_down)] > 0;

    return std::vector<float>{
            // Danger straight (same direction)
            (float) ((is_direction_right && is_collision(point_right)) ||
                      (is_direction_left && is_collision(point_left)) ||
                      (is_direction_up && is_collision(point_up)) ||
                      (is_direction_down && is_collision(point_down))),
            // Danger right (danger is at the right of current direction)
            (float) ((is_direction_right && is_collision(point_down)) ||
                      (is_direction_left && is_collision(point_up)) ||
                      (is_direction_up && is_collision(point_right)) ||
                      (is_direction_down && is_collision(point_left))),
            // Danger left (danger is at the left of current direction)
            (float) ((is_direction_right && is_collision(point_up)) ||
                      (is_direction_left && is_collision(point_down)) ||
                      (is_direction_up && is_collision(point_left)) ||
                      (is_direction_down && is_collision(point_right))),
            // current direction
            (float) is_direction_right,
            (float) is_direction_left,
            (float) is_direction_up,
            (float) is_direction_down,
            // food location
            (float) is_food_left,
            (float) is_food_right,
            (float) is_food_up,
            (float) is_food_down,
            // point reaches food
            (float) right_reaches_food,
            (float) left_reaches_food,
            (float) up_reaches_food,
            (float) down_reaches_food,
    };
}

void SnakeGameAI::reset()
{
    curr_action = ACTION_RIGHT;
    head.x = w / 2;
    head.y = h / 2;

    snake.clear();
    snake.push_back(head);
    snake.push_back({head.x - 1, head.y});
    snake.push_back({head.x - 2, head.y});

    score = 0;
    place_food();
    frame_iteration = 0;
}

void SnakeGameAI::move(action_t action)
{
    action_t newAction = action;
    if ((curr_action == ACTION_RIGHT && action == ACTION_LEFT) ||
        (curr_action == ACTION_LEFT && action == ACTION_RIGHT) ||
        (curr_action == ACTION_DOWN && action == ACTION_UP) ||
        (curr_action == ACTION_UP && action == ACTION_DOWN))
    {
        newAction = curr_action;
    }
    switch (newAction)
    {
        case ACTION_RIGHT:
        {
            head.x += 1;
            break;
        }
        case ACTION_LEFT:
        {
            head.x -= 1;
            break;
        }
        case ACTION_UP:
        {
            head.y -= 1;
            break;
        }
        case ACTION_DOWN:
        {
            head.y += 1;
            break;
        }
        default:
            assert(0);
            break;
    }
    curr_action = newAction;
}

StepResult SnakeGameAI::get_step(int action)
{
    StepResult result;
    ++frame_iteration;

    move((action_t) action);
    snake.insert(snake.begin(), head);
    update_field();

    if (is_collision(head))
    {
        result.game_over = true;
        result.reward = -10;
        return result;
    }

    if (head == food)
    {
        ++score;
        place_food();
        result.reward = 10;
        return result;
    }
    snake.pop_back();
    return result;
}

size_t SnakeGameAI::get_state_size() const
{
    return 15;
}

size_t SnakeGameAI::get_action_count() const
{
    return ACTION_COUNT;
}

int SnakeGameAI::get_score() const
{
    return score;
}
