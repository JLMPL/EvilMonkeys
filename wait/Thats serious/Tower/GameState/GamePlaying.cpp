#include "GamePlaying.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

void GamePlaying::init()
{
    m_world.init();
}

void GamePlaying::exit()
{
    m_world.exit();
    gRenderer3D.clearAllData();
    gRenderer2D.clearAllData();
}

void GamePlaying::update(seconds deltaTime)
{
    m_world.update(deltaTime);
}

void GamePlaying::sendMessage(const Message& message)
{
    m_world.sendMessage(message);

    if (message.type == Message::Type::KeyPressed)
    {
        if (message.keyboard.key == Config::Controls::Exit)
        {
            Message message(Message::Type::GameStateExit);
            MessageBus::queueMessage(message);
        }
    }
}
