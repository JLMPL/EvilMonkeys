#include "GameOver.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

void GameOver::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    m_over.init(&m_font);
    m_over.setString("That would be it. For now at least...", 0.5);
    m_over.setPosition(vec2(
        Config::Display::Width * 0.5 - m_over.getWidth() * 0.5,
        Config::Display::Height * 0.5 - 100
    ));

    gRenderer2D.registerText(&m_over);

    m_exit.init(&m_font);
    m_exit.setString("Press [Some Keys] To Exit Game", 0.3);
    m_exit.setPosition(vec2(
        Config::Display::Width * 0.5 - m_exit.getWidth() * 0.5,
        Config::Display::Height * 0.5
    ));

    gRenderer2D.registerText(&m_exit);

    m_rect.init(vec2(Config::Display::Width, Config::Display::Height));
    m_rect.setColor(vec4(0,0,0,0.5));
    m_rect.setPosition(vec2(0));

    gRenderer2D.registerRectangle(&m_rect);
}

void GameOver::exit()
{
    gRenderer2D.clearAllData();
}

void GameOver::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        Message message(Message::Type::GameStateExit);
        MessageBus::queueMessage(message);
    }
}

void GameOver::update(seconds deltaTime)
{
}
