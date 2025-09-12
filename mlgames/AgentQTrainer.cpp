#include <mlgames/AgentQTrainer.h>


AgentQTrainer::AgentQTrainer(
        LinearNN *model_, torch::optim::Optimizer *optimizer_, const c10::DeviceType device_,
        size_t batch_size_, float gamma_)
{
    device = device_;
    model = model_;
    optimizer = optimizer_;
    batch_size = batch_size_;
    gamma = gamma_;
}


std::vector<int>
AgentQTrainer::get_play(const std::vector<float> &state, size_t count_samples) const
{
    const auto state0 = torch::tensor(state, torch::kFloat)
                                .reshape({(long) count_samples, (long) (*model)->input_size})
                                .to(device, torch::kFloat);
    (*model)->eval();
    auto prediction = torch::Tensor();
    {
        torch::NoGradGuard no_grad;
        prediction = (*model)->forward(state0);
    }
    // std::cout << prediction.sizes() << "\n";
    // prediction.size(0) -> count_samples
    // prediction.size(1) -> (*model)->output_size
    std::vector<int> action(prediction.size(0) * prediction.size(1), 0);
    for (int r = 0; r < prediction.size(0); ++r)
    {
        const int index = torch::argmax(prediction[r]).item().toInt();
        action[r * prediction.size(1) + index] = 1;
    }
    return action;
}


std::vector<int>
AgentQTrainer::get_action(const std::vector<float> &state, size_t count_samples) const
{
    const int epsilon = 80 - number_of_games;

    // epsilon = tradeoff between exploration vs exploitation
    // in the beginning this is true for some time, later self.number_of_games is larger
    if (rand() % 200 < epsilon)
    {
        std::vector<int> action((*model)->output_size * count_samples, 0);
        action[rand() % (*model)->output_size] = 1;
        return action;
    }

    return get_play(state, count_samples);
}


void AgentQTrainer::train_short_memory(
        const std::vector<float> &state, const std::vector<int> &action, int reward,
        const std::vector<float> &next_state, bool game_over)
{
    train_step(1, state, action, {reward}, next_state, {game_over});
}


void AgentQTrainer::remember(
        const std::vector<float> &state, const std::vector<int> &action, const int reward,
        const std::vector<float> &next_state, const bool game_over)
{
    memory_deque.push_back(
            {(*model)->input_size, (*model)->output_size, state, action, reward, next_state,
             game_over});
}


void AgentQTrainer::train_long_memory()
{
    std::vector<MemoryData> samples;
    size_t sample_size;

    if (memory_deque.size() > batch_size)
    {
        sample_size = batch_size;
        samples.resize(sample_size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::ranges::sample(
                memory_deque.begin(), memory_deque.end(), samples.begin(), sample_size, gen);
    }
    else
    {
        sample_size = memory_deque.size();
        samples.reserve(sample_size);
        samples.insert(samples.end(), memory_deque.begin(), memory_deque.end());
    }

    const size_t num_state = samples[0].num_state;
    const size_t num_action = samples[0].num_action;

    std::vector<float> states;
    std::vector<int> actions;
    std::vector<int> rewards;
    std::vector<float> next_states;
    std::vector<bool> game_overs;

    states.reserve(sample_size * num_state);
    actions.reserve(sample_size * num_action);
    rewards.reserve(sample_size);
    next_states.reserve(sample_size * num_state);
    game_overs.reserve(sample_size);

    for (size_t i = 0; i < sample_size; ++i)
    {
        states.insert(states.end(), samples[i].state.begin(), samples[i].state.end());
        next_states.insert(
                next_states.end(), samples[i].next_state.begin(), samples[i].next_state.end());
        actions.insert(actions.end(), samples[i].action.begin(), samples[i].action.end());
        rewards.push_back(samples[i].reward);
        game_overs.push_back(samples[i].game_over);
    }

    train_step(sample_size, states, actions, rewards, next_states, game_overs);
}

void AgentQTrainer::train_step(
        size_t count_samples, const std::vector<float> &old_states_,
        const std::vector<int> &actions_, const std::vector<int> &rewards_,
        const std::vector<float> &new_states_, const std::vector<bool> &dones_)
{
    const torch::Tensor old_states =
            torch::tensor(old_states_, torch::kFloat)
                    .reshape({(long) count_samples, (long) (*model)->input_size})
                    .to(device, torch::kFloat);
    const torch::Tensor actions =
            torch::tensor(actions_, torch::kInt)
                    .reshape({(long) count_samples, (long) (*model)->output_size})
                    .to(device, torch::kFloat);
    const torch::Tensor new_states =
            torch::tensor(new_states_, torch::kFloat)
                    .reshape({(long) count_samples, (long) (*model)->input_size})
                    .to(device, torch::kFloat);

    // 1: predict Q values with current state
    (*model)->train();
    torch::Tensor pred_action = (*model)->forward(old_states);

    // 2: Q_new = r + y * max(next_predicted_Q_value) -> only do this if not done
    (*model)->eval();
    torch::Tensor target = pred_action.detach().clone();
    {
        torch::NoGradGuard no_grad;
        for (size_t index = 0; index < dones_.size(); ++index)
        {
            auto q_new = rewards_[index];
            const long max_index = torch::argmax(actions[index]).item().toLong();
            if (!dones_[index])
            {
                q_new = q_new +
                        gamma * (*model)->forward(new_states[index])[max_index].item().toFloat();
            }
            target[index][max_index] = q_new;
        }
    }

    (*model)->train();
    optimizer->zero_grad();
    const torch::Tensor loss = criterion(pred_action, target);
    loss.backward();
    optimizer->step();
}
