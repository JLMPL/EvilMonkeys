#include "Input.hpp"
#include "Core/Config.hpp"
#include "Messages/MessageBus.hpp"

void Input::sendKeyMessage(i32 key)
{
    Message message(Message::Type::KeyPressed);
    message.keyboard.key = key;
    MessageBus::queueMessage(message);
}

void Input::checkKey(i32 key)
{
    if (m_keys[key])
        sendKeyMessage(key);
}

void Input::update()
{
    m_keys = SDL_GetKeyboardState(nullptr);

    checkKey(Config::Controls::Up);
    checkKey(Config::Controls::Down);
    checkKey(Config::Controls::Left);
    checkKey(Config::Controls::Right);
    checkKey(Config::Controls::Jump);
    checkKey(Config::Controls::Prep);
    checkKey(Config::Controls::Sprint);
    checkKey(Config::Controls::Return);
    checkKey(Config::Controls::Exit);
    checkKey(Config::Controls::Cheat);

    SDL_GetRelativeMouseState(&m_mouseRelative.x, &m_mouseRelative.y);

    if (m_mouseRelative != vec2i(0))
    {
        Message message(Message::Type::MouseMoved);

        message.mouse.relx = m_mouseRelative.x;
        message.mouse.rely = m_mouseRelative.y;

        MessageBus::queueMessage(message);
    }

    auto buttons = SDL_GetMouseState(nullptr, nullptr);
    bool left = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    bool right = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

    if (left)
    {
        Message message(Message::Type::MousePressed);
        message.mouse.left = true;
        MessageBus::queueMessage(message);
    }
    if (right)
    {
        Message message(Message::Type::MousePressed);
        message.mouse.right = true;
        MessageBus::queueMessage(message);
    }
}
