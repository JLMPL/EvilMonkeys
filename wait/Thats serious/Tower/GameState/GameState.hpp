#pragma once
#include "Core/Types.hpp"
#include <memory>

struct Message;

class GameState
{
    public:
        using Ptr = std::unique_ptr<GameState>;

        enum Type
        {
            Menu,
            Help,
            Settings,
            Playing,
            Over,

            Count
        };

        GameState() = default;
        ~GameState() = default;

        virtual void init() = 0;
        virtual void update(seconds deltaTime) = 0;
        virtual void exit() = 0;

        virtual void sendMessage(const Message& message) = 0;
    private:
};
