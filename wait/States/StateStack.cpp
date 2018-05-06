#include "StateStack.hpp"
#include "SplashState.hpp"
#include "GameState.hpp"

void StateStack::init(Renderer* renderer)
{
    m_renderer = renderer;
    
    push(State::Type::Game);
}

void StateStack::processEvents(const SDL_Event& event)
{
    for (auto i = m_states.rbegin(); i != m_states.rend(); i++)
        if (!(*i)->processEvents(event))
            break;
        
    applyChanges();
}

void StateStack::update(float deltaTime)
{
    for (auto i = m_states.rbegin(); i != m_states.rend(); i++)
        if (!(*i)->update(deltaTime))
            break;

    applyChanges();
}

void StateStack::render()
{
    for (auto& state : m_states)
        state->render();
}

void StateStack::push(State::Type state)
{
    Change ch = {Change::Type::Push, state};

    m_changes.push_back(ch);
}

void StateStack::pop()
{
    Change ch = {Change::Type::Pop};

    m_changes.push_back(ch);
}

void StateStack::clear()
{
    Change ch = {Change::Type::Clear};

    m_changes.push_back(ch);
}

bool StateStack::isEmpty()
{
    return m_states.empty();
}

State::Ptr StateStack::createState(State::Type type)
{
    switch (type)
    {
        case State::Type::Splash:
            return State::Ptr(new SplashState(this, m_renderer));
        break;
        case State::Type::Main:
            // return State::Ptr(new SplashState(this));
        break;
        case State::Type::Game:
            return State::Ptr(new GameState(this, m_renderer));
        break;
    }
}

void StateStack::applyChanges()
{
    for (auto& change : m_changes)
    {
        switch (change.type)
        {
            case Change::Type::Push:
                m_states.push_back(createState(change.state));
            break;
            case Change::Type::Pop:
                m_states.pop_back();
            break;
            case Change::Type::Clear:
                m_states.clear();
            break;
        };
    }

    m_changes.clear();
}
