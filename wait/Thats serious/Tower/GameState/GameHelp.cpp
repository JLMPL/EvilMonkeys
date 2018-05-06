#include "GameHelp.hpp"
#include "Messages/MessageBus.hpp"
#include "Render/Renderer3D.hpp"
#include "Render/Renderer2D.hpp"
#include "Core/Config.hpp"

void GameHelp::init()
{
    m_camera.init();
    gRenderer3D.setActiveCamera(&m_camera);

    m_font.loadFromFile("Fonts/dis.fnt");

    m_text.init(&m_font);
    m_text.setString("Back", 0.5);
    m_text.setPosition(vec2(
        Config::Display::Width * 0.5 - m_text.getWidth() * 0.5,
        Config::Display::Height - 100
    ));

    m_rect.init({192,48});
    m_rect.setColor({0,0,0,0.5});
    m_rect.setPosition(vec2(
        Config::Display::Width * 0.5 - 192 * 0.5,
        Config::Display::Height - 100
    ));

    m_help.init(&m_font);
    m_help.setString("No help for you!", 0.75);
    m_help.setPosition(vec2(
        Config::Display::Width * 0.5 - m_help.getWidth() * 0.5,
        200
    ));

    m_back.init({Config::Display::Width, Config::Display::Height});
    m_back.setColor({0,0,0,0.5});

    gRenderer2D.registerRectangle(&m_back);
    gRenderer2D.registerRectangle(&m_rect);
    gRenderer2D.registerText(&m_text);
    gRenderer2D.registerText(&m_help);

    m_timer = seconds(0);
}

void GameHelp::sendMessage(const Message& message)
{
    if (message.type == Message::Type::KeyPressed)
    {
        if (m_timer > seconds(0.2))
        {
            if (message.keyboard.key == Config::Controls::Exit ||
                message.keyboard.key == Config::Controls::Return)
            {
                Message message(Message::Type::GameStateSet);
                message.state = i32(GameState::Type::Menu);
                MessageBus::queueMessage(message);
                m_timer = seconds(0);
            }
        }
    }
}

void GameHelp::update(seconds deltaTime)
{
    m_timer += deltaTime;
}

void GameHelp::exit()
{
    gRenderer2D.clearAllData();
}
