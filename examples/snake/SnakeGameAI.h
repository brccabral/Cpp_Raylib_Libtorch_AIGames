#pragma once

#include <mlgames/AbstractGame.h>


typedef struct Vector2i
{
    int x;
    int y;
} Vector2i;


class SnakeGameAI : public AbstractGame
{
public:

    SnakeGameAI();

    [[nodiscard]] size_t get_state_size() const override;
    [[nodiscard]] size_t get_action_count() const override;
    [[nodiscard]] int get_score() const override;
    StepResult get_step(int action) override;
    [[nodiscard]] std::vector<float> get_state() const override;
    void reset() override;

    typedef enum action_t
    {
        ACTION_UP,
        ACTION_DOWN,
        ACTION_LEFT,
        ACTION_RIGHT,
        ACTION_COUNT
    } action_t;

    std::vector<Vector2i> snake;
    Vector2i food{};
    action_t curr_action = ACTION_RIGHT;

    int w = 32;
    int h = 24;
    Vector2i head = Vector2i(w / 2.0f, h / 2.0f);

private:

    struct FieldLoc
    {
        Vector2i p;
        int distance;
    };

    int score = 0;
    size_t state_size = 15;
    int frame_iteration = 0;
    int distance_field[32 * 24]{-1};
    Vector2i p_right = {1, 0};
    Vector2i p_left = {-1, 0};
    Vector2i p_up = {0, -1};
    Vector2i p_down = {0, 1};
    void new_food();
    void place_food();
    [[nodiscard]] bool is_collision(Vector2i pt) const;
    void move(action_t action);
    void update_field();
    [[nodiscard]] size_t index_from_location(Vector2i pt) const;
};
