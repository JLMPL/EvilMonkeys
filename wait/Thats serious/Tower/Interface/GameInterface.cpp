#include "GameInterface.hpp"
#include "Messages/Message.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

void GameInterface::init()
{
    m_back.init(vec2(Config::Display::Width, 64));
    m_back.setColor(vec4(0,0,0,0.5));
    m_back.setPosition(vec2(0));

    gRenderer2D.registerRectangle(&m_back);

    m_health.init(vec2(16,16), vec2(Config::Display::Width / 2 - 32, 32), vec3(0,0.7,0.2), 100);
    m_health.setValue(100);

    m_enemyHealth.init(vec2(Config::Display::Width / 2 + 16, 16), vec2(Config::Display::Width / 2 - 32, 32), vec3(0,0.7,0.2), 100, true);
    m_enemyHealth.setValue(100);

    m_font.loadFromFile("Fonts/dis.fnt");
    m_text.init(&m_font);
    m_text.setString("The Player", 0.35);
    m_text.setPosition(vec2(16 + 6, 16));

    gRenderer2D.registerText(&m_text);
}

void GameInterface::exit()
{
    gRenderer2D.unregisterText(&m_text);
    gRenderer2D.unregisterRectangle(&m_back);
}

void GameInterface::sendMessage(const Message& message)
{
    if (message.type == Message::Type::InterfaceSetEnemyHealth)
    {
        m_enemyHealth.setValue(message.ivalue);
    }
    else if (message.type == Message::Type::InterfaceSetHealth)
    {
        m_health.setValue(message.ivalue);
    }
}
