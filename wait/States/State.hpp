#ifndef STATE_HPP
#define STATE_HPP
#include "Core/Types.hpp"
#include <memory>

union SDL_Event;

class StateStack;

class State
{
    public:
        using Ptr = std::unique_ptr<State>;

        enum class Type
        {
            Splash,
            Main,
            Game
        };

        State() = default;
        virtual ~State() = default;

        // virtual void init(StateStack* stack) = 0;
        virtual bool processEvents(const SDL_Event& event) = 0;
        virtual bool update(float deltaTime) = 0;
        virtual void render() = 0;

    protected:
        StateStack* m_stateStack = nullptr;
};

#endif