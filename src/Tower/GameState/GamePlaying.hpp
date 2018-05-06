#pragma once
#include "GameState.hpp"
#include "Game/World.hpp"

class GamePlaying : public GameState
{
    public:
        GamePlaying() = default;
        ~GamePlaying() = default;

        void init() override final;
        void update(seconds deltaTime) override final;
        void exit() override final;

        void sendMessage(const Message& message) override final;

    private:
        World m_world;
};
