#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <raylib.h>
#include "SnakeGameAI.h"
#define MANUAL 0

#if !MANUAL
#include <mlgames/AgentQTrainer.h>
#define LR (0.001)
#define GAMMA (0.9)
#define HIDDEN_SIZE (256)
// memory usage (if CPU -> RAM, if GPU -> GPU_MEM)
#define BATCH_SIZE (1000)
#endif

int main()
{
    // NOLINTNEXTLINE
    srand(time(NULL));

#if !MANUAL
    c10::DeviceType device = torch::kCPU;
    if (torch::cuda::is_available())
    {
        printf("Using CUDA\n");
        device = torch::kCUDA;
    }
    else
    {
        printf("Using CPU\n");
        device = torch::kCPU;
    }
#endif

    SnakeGameAI game;

#if !MANUAL
    auto model = LinearNN(
            game.get_state_size(), game.get_action_count(), std::vector<size_t>{HIDDEN_SIZE});
    model->to(device);
    model->train();
    auto optimizer = torch::optim::Adam(model->parameters(), torch::optim::AdamOptions{LR});

    AgentQTrainer agent(&model, &optimizer, device, BATCH_SIZE, GAMMA);
#endif

    constexpr int BLOCK_SIZE = 20;

    int best_score = 0;

    InitWindow(640, 480, "SnakeAI");
#if MANUAL
    SetTargetFPS(10);
#endif


    while (!WindowShouldClose())
    {
#if MANUAL
        SnakeGameAI::action_t game_action = game.curr_action;
        if (IsKeyPressed(KEY_UP))
        {
            game_action = SnakeGameAI::ACTION_UP;
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            game_action = SnakeGameAI::ACTION_DOWN;
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            game_action = SnakeGameAI::ACTION_LEFT;
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            game_action = SnakeGameAI::ACTION_RIGHT;
        }
#else
        auto state_old = game.get_state();
        auto action = agent.get_action(state_old, 1);

        SnakeGameAI::action_t game_action{};
        for (size_t i = 0; i < action.size(); ++i)
        {
            if (action[i] == 1)
            {
                game_action = (SnakeGameAI::action_t) i;
                break;
            }
        }
#endif

        auto [reward, game_over] = game.get_step(game_action);
        if (game_over)
        {
            int score = game.get_score();
            if (score > best_score)
            {
                best_score = score;
            }
            game.reset();
        }

#if !MANUAL
        auto state_new = game.get_state();
        agent.train_short_memory(state_old, action, reward, state_new, game_over);

        agent.remember(state_old, action, reward, state_new, game_over);

        if (game_over)
        {
            ++agent.number_of_games;
            int score = game.get_score();
            if (score > best_score)
            {
                best_score = score;
                save_model<>(model);
            }

            printf("Game %d Score %d Record %d time %.2f batch %d max memory %d\n",
                   agent.number_of_games, score, best_score, GetTime(), BATCH_SIZE, MAX_MEMORY);

            // train long memory (also called replay memory, or experience replay)
            agent.train_long_memory();
            game.reset();
        }
#endif

        BeginDrawing();
        ClearBackground(BLACK);
        for (auto [x, y]: game.snake)
        {
            DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLUE);
            DrawRectangle(
                    x * BLOCK_SIZE + 4, y * BLOCK_SIZE + 4, BLOCK_SIZE - 8, BLOCK_SIZE - 8,
                    SKYBLUE);
        }
        DrawRectangle(
                game.food.x * BLOCK_SIZE, game.food.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RED);
        DrawText(TextFormat("Score: %d", game.get_score()), 0, 0, 20, WHITE);
        DrawText(TextFormat("Record: %d", best_score), 0, 20, 20, WHITE);
        DrawText(TextFormat("Time: %.2f", GetTime()), 0, 40, 20, WHITE);
#if !MANUAL
        DrawText(TextFormat("Batch: %d", BATCH_SIZE), 0, 60, 20, WHITE);
        DrawText(TextFormat("Max memory: %d", MAX_MEMORY), 0, 80, 20, WHITE);
#endif

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
