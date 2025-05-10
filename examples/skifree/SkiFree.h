#pragma once
#include <list>
#include <raylib.h>
#include <vector>
#include <chrono>

class SkiObject
{
public:

    SkiObject() = default;
    ~SkiObject() = default;

    void update(const std::vector<Rectangle> &frames, float speed_limit);
    Rectangle get_location() const;
    BoundingBox get_collision_box() const;

    enum object_type
    {
        // characters
        TYPE_SKIER = 0,
        TYPE_YETI,
        // fixed positions
        TYPE_SLALOM_ARROW_LEFT,
        TYPE_SLALOM_ARROW_RIGHT,
        TYPE_START_LEFT,
        TYPE_START_RIGHT,
        TYPE_FINISH_LEFT,
        TYPE_FINISH_RIGHT,
        TYPE_SLALOM_SIGN,
        TYPE_TREE_SLALOM_SIGN,
        TYPE_FREESTYLE_SIGN,
        TYPE_RESTART_PAUSE_SIGN,
        TYPE_NUMPAD,
        // lift
        TYPE_LIFT_POLE,
        TYPE_LIFT_CHAIR,
        // special
        TYPE_URINE,
        // obstacles
        TYPE_MOGUL_GROUP,
        TYPE_NOVICE,
        TYPE_DOG,
        TYPE_SNOWBOARDER,
        TYPE_TREE_WALK,
        TYPE_ROCK,
        TYPE_STUMP,
        TYPE_MOGUL_SMALL,
        TYPE_MOGUL_LARGE,
        TYPE_TREE_SMALL,
        TYPE_TREE_DRIED,
        TYPE_TREE_LARGE,
        TYPE_RAINBOW,
    };

    enum object_state
    {
        STATE_PLAYER_LEFT,
        STATE_PLAYER_RIGHT,
        STATE_PLAYER_30_LEFT,
        STATE_PLAYER_30_RIGHT,
        STATE_PLAYER_60_LEFT,
        STATE_PLAYER_60_RIGHT,
        STATE_PLAYER_OUCH,
        STATE_PLAYER_SIT,
        STATE_PLAYER_JUMP,
        STATE_PLAYER_HIT,
        STATE_PLAYER_DOWN,
        STATE_PLAYER_EATING,
        STATE_SLALOM_LEFT,
        STATE_SLALOM_RIGHT,
        STATE_SLALOM_SUCCESS,
        STATE_SLALOM_FAIL,
        STATE_NOVICE_NORMAL,
        STATE_NOVICE_SIT,
        STATE_NOVICE_GROUND,
        STATE_DOG_RIGHT,
        STATE_DOG_HIT,
        STATE_SNOWBOARDER_LEFT,
        STATE_SNOWBOARDER_RIGHT,
        STATE_SNOWBOARDER_360,
        STATE_TREE_DRIED_NORMAL,
        STATE_TREE_DRIED_FIRE,
        STATE_LIFT_CHAIR_FULL,
        STATE_LIFT_CHAIR_NOVICE,
        STATE_LIFT_CHAIR_EMPTY,
        STATE_YETI_LEFT,
        STATE_YETI_RIGHT,
        STATE_YETI_UP,
        STATE_YETI_HAPPY_1,
        STATE_YETI_HAPPY_2,
        STATE_YETI_EATING,
        STATE_TREE_WALK_LEFT,
        STATE_TREE_WALK_RIGHT,
    };

    object_type type{};
    Vector2 position{};
    float offset_y{};
    Vector2 direction{};
    Vector2 velocity{};
    size_t current_frame_index{};
    Rectangle current_frame_rectangle{};
    object_state state{};
    int state_countdown{};
    double speed{};
    int jump{};
};

class SkiFree
{
public:

    SkiFree();
    ~SkiFree();

    void inputs();
    void update();
    void draw() const;
    void reset();
    SkiObject create_snowboarder();
    void collisions_manager();

private:

    Texture2D all_textures{};
    std::vector<Rectangle> frames{};

    enum game_mode_t
    {
        MODE_PRACTICE = 0,
        MODE_SLALOM,
        MODE_FREESTYLE,
        MODE_TREE_SLALOM
    };

    std::list<SkiObject *> long_live_objects{};
    std::list<SkiObject> lift_objects{};
    std::list<SkiObject> slalom_flags_objects{};

    // teleported at 2050
    std::list<SkiObject *> characters_objects{};
    std::list<SkiObject> short_live_objects{};

    Camera2D camera{};
    SkiObject player;
    SkiObject slalom_sign;
    SkiObject freestyle_sign;
    SkiObject tree_slalom_sign;
    SkiObject restart_pause_sign;
    SkiObject numpad_sign;
    SkiObject start_left_freestyle_sign;
    SkiObject start_right_freestyle_sign;
    SkiObject start_left_slalom_sign;
    SkiObject start_right_slalom_sign;
    SkiObject start_left_tree_slalom_sign;
    SkiObject start_right_tree_slalom_sign;
    SkiObject finish_left_slalom_sign;
    SkiObject finish_right_slalom_sign;
    SkiObject finish_left_tree_slalom_sign;
    SkiObject finish_right_tree_slalom_sign;
    SkiObject finish_left_freestyle_sign;
    SkiObject finish_right_freestyle_sign;
    SkiObject yeti_1;
    SkiObject yeti_2;

    game_mode_t current_mode{};

    Rectangle current_area{};
    float area_size = 3000.0f;
    size_t num_elements_in_area = 140;

    void manage_objects();
    void player_hit(const BoundingBox &other_box);
    void player_jump(const int jump_height);
    void yeti_chase(SkiObject *yeti, const float speed);
    void yeti_stop(SkiObject *yeti);
    void start_slalom();
    void start_freestyle();
    void start_tree_slalom();
    bool player_crossed_down(const float threshold, const Vector2 pos_before);
    bool player_crossed_up(const float threshold, const Vector2 pos_before);
    void slalom_manager(const game_mode_t current_mode, const Vector2 pos_before);

    bool is_paused{};
    bool is_waiting_action = true;

    float speed_limit = 25;

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time = start_time;
    float display_distance{};
    float end_distance{};
};
