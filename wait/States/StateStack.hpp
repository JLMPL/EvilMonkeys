#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP
#include "State.hpp"
#include "Render/Renderer.hpp"
#include <vector>

class StateStack
{
    public:
        StateStack() = default;

        void init(Renderer* renderer);
        void processEvents(const SDL_Event& event);
        void update(float deltaTime);
        void render();

        void push(State::Type state);
        void pop();
        void clear();

        bool isEmpty();

    private:
        struct Change
        {
            enum class Type
            {
                Push,
                Pop,
                Clear
            };

            Type type;
            State::Type state;
        };

        State::Ptr createState(State::Type type);
        void applyChanges();

    private:
        Renderer*                  m_renderer = nullptr;
        std::vector<State::Ptr>    m_states;
        std::vector<Change>        m_changes;
};

#endif